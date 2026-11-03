/*
 * A066147  Numbers k such that all 3 of EulerPhi(k) + 1, d(k) + 1, sigma(k) +
 *          1 are simultaneously prime (d(k) denotes the number of divisors of
 *          k).
 * https://oeis.org/A066147
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 74 terms through n = 980 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem, num_divisors, totient_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A066147: a(n) is the nth integer k such that φ(k)+1, τ(k)+1 and σ(k)+1 are all prime. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include <stddef.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/num_divisors.h"
#include "../utils/totient_seq.h"
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

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

/* Cache for the found terms */
static seq_t *seq_cache = NULL;
static size_t seq_cap  = 0;   /* current capacity of seq_cache          */
static size_t seq_len  = 0;   /* number of terms stored in seq_cache    */
static uint64_t next_check = 1; /* next integer to test for candidacy   */

/* Extend the cache to hold at least 'needed' entries. */
static void grow_cache(size_t needed)
{
    if (needed <= seq_cap)
        return;
    size_t new_cap = seq_cap == 0 ? 16 : seq_cap * 2;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_cache = mem_resize(seq_t, seq_cache, new_cap);
    seq_cap = new_cap;
}

/* Return true if k satisfies the three primality conditions. */
static bool candidate_ok(uint64_t k)
{
    /* φ(k)+1 must be prime */
    seq_t phi = totient_seq((seq_t)k);
    if (!isPrimeGeneric(phi + (seq_t)1))
        return false;

    /* σ(k)+1 must be prime */
    seq_t sigma = sigma_of((idx_t)k);
    if (!isPrimeGeneric(sigma + (seq_t)1))
        return false;

    /* τ(k)+1 must be prime – compute divisor count */
    uint64_t divcnt = num_divisors(k);
    if (!isPrimeGeneric((seq_t)(divcnt + 1)))
        return false;

    return true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)   /* undefined for 0, return 0 as a safety value */
        return (seq_t)0;

    uint64_t target = (uint64_t)n;

    /* Generate terms until we have at least `n` of them */
    while (seq_len < target) {
        if (candidate_ok(next_check)) {
            grow_cache(seq_len + 1);
            seq_cache[seq_len++] = (seq_t)next_check;
        }
        ++next_check;
    }
    return seq_cache[n - 1];
}
