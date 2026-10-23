/*
 * A176238  Natural numbers n such that d(d(n)+1) > 2.
 * https://oeis.org/A176238
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   _smallest_prime_factor, isPrimeGeneric
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

#include "../utils/_smallest_prime_factor.h"
#include "../utils/isPrimeGeneric.h"

/* Divide n by its smallest prime factor repeatedly to
   compute the number of divisors τ(n). */
static seq_t divisor_count_u64(uint64_t n)
{
    seq_t tau = 1;
    uint64_t tmp = n;
    while (tmp > 1) {
        uint64_t p = _smallest_prime_factor(tmp);
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
                seq_cache = (seq_t *)realloc(seq_cache, seq_cap * sizeof(seq_t));
            }
            seq_cache[seq_len++] = (seq_t)last_tested;
        }
    }
    return seq_cache[(size_t)(n - 1)];
}
