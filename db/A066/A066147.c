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
 * Helpers:   calc_phi, divisor_count_u64, isPrimeGeneric, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A066147: a(n) is the nth integer k such that φ(k)+1, τ(k)+1 and σ(k)+1 are all prime. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "../utils/calc_phi.h"          /* seq_t calc_phi(seq_t m) */
#include "../utils/isPrimeGeneric.h"    /* bool isPrimeGeneric(seq_t n) */
#include "../utils/sigma_of.h"          /* seq_t sigma_of(idx_t n) */
#include "../utils/divisor_count_u64.h" /* uint64_t divisor_count_u64(uint64_t n) */

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
    seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* Return true if k satisfies the three primality conditions. */
static bool candidate_ok(uint64_t k)
{
    /* φ(k)+1 must be prime */
    seq_t phi = calc_phi((seq_t)k);
    if (!isPrimeGeneric(phi + (seq_t)1))
        return false;

    /* σ(k)+1 must be prime */
    seq_t sigma = sigma_of((idx_t)k);
    if (!isPrimeGeneric(sigma + (seq_t)1))
        return false;

    /* τ(k)+1 must be prime – compute divisor count */
    uint64_t divcnt = divisor_count_u64(k);
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
