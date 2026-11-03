/*
 * A176238  Natural numbers n such that d(d(n)+1) > 2.
 * https://oeis.org/A176238
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 72 terms through n = 953 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem, smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: natural numbers n such that d(d(n)+1) > 2.
   This implementation performs a sequential search, keeping
   the numbers that satisfy the condition. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/smallest_prime_factor.h"
#include "../utils/mem.h"

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

/* Divide n by its smallest prime factor repeatedly to
   compute the number of divisors τ(n). */
static seq_t divisor_count_u64(uint64_t n)
{
    seq_t tau = 1;
    uint64_t tmp = n;
    while (tmp > 1) {
        uint64_t p = smallest_prime_factor(tmp);
        unsigned exp = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++exp;
        }
        tau *= (seq_t)(exp + 1);
    }
    return tau;
}

/* Cached sequence values. */
static seq_t *seq_cache   = NULL;
static size_t   seq_len   = 0;
static size_t   seq_cap   = 0;
/* Largest n that has already been tested. */
static uint64_t last_tested = 1;

/* Main sequence function: a(idx_t n) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for 0, return 0 for safety */

    /* Ensure cache holds at least n terms. */
    while (seq_len < (size_t)n) {
        ++last_tested;
        seq_t tau = divisor_count_u64(last_tested);
        seq_t tau_plus = tau + (seq_t)1;
        /* Include the number if d(d(n)+1) > 2, i.e. if τ+1 is composite. */
        if (!isPrimeGeneric(tau_plus)) {
            /* Grow cache if needed. */
            if (seq_len == seq_cap) {
                seq_cap = (seq_cap == 0) ? 16 : seq_cap * 2;
                seq_cache = mem_resize(seq_t, seq_cache, seq_cap);
            }
            seq_cache[seq_len++] = (seq_t)last_tested;
        }
    }
    return seq_cache[(size_t)(n - 1)];
}
