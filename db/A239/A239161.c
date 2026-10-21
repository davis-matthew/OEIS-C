/*
 * A239161  Numbers k such that the ratio (sum of divisors of k) /(sum of
 *          divisors d of k with d <= sqrt(k)) is an integer.
 * https://oeis.org/A239161
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 170, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64, gen_small_divs
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A239161: k such that σ(k) / σ_{≤√k}(k) is an integer.
   The function generates terms by testing successive integers k,
   storing those that satisfy the divisibility condition. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/gen_small_divs.h"

/* --------- helpers --------------------------------------------- */

/* Integer square root for 64‑bit values. */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0;
    idx_t high = ((idx_t)1 << 32); /* sqrt(2^64‑1) < 2^32            */
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if (mid <= n / mid)   /* mid*mid <= n                         */
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* Static cache for sequence values. */
static seq_t *seq_val     = NULL;
static size_t seq_len     = 0;        /* number of cached terms */
static size_t seq_cap     = 0;        /* allocated capacity      */
static uint64_t last_k    = 0;        /* last integer tested    */

/* Ensure at least new_cap entries can be stored. */
static void ensure_seq_cap(size_t new_cap)
{
    if (new_cap <= seq_cap) return;
    size_t cap = seq_cap ? seq_cap : 1;
    while (cap < new_cap) cap <<= 1;
    seq_val = (seq_t *)realloc(seq_val, cap * sizeof(seq_t));
    seq_cap = cap;
}

/* --------- sequence function ------------------------------------- */

/* Return the nth term (offset 1).  Any previously computed values
   are memoized, so calls need not be monotonic. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for offset 0 */

    /* Extend cache until we have at least n terms. */
    while (seq_len < (size_t)n) {
        ++last_k;
        uint64_t k = last_k;

        /* Factor k. */
        uint64_t primes[64];
        uint64_t exps  [64];
        size_t   pcnt  = 0;
        factor_u64(k, primes, exps, &pcnt);

        /* Compute σ(k). */
        seq_t sigma_total = (seq_t)1;
        for (size_t i = 0; i < pcnt; ++i) {
            seq_t term   = (seq_t)1;
            seq_t pk     = (seq_t)1;
            for (uint64_t e = 0; e < exps[i]; ++e) {
                pk *= (seq_t)primes[i];
                term += pk;
            }
            sigma_total *= term;
        }

        /* Compute σ_{≤√k}(k). */
        seq_t sigma_small = (seq_t)0;
        uint64_t sqrtk = isqrt_u64(k);
        gen_small_divs(0, 1, sqrtk, primes, exps, pcnt, &sigma_small);

        /* Check divisibility. */
        if (sigma_small != 0 && sigma_total % sigma_small == 0) {
            ensure_seq_cap(seq_len + 1);
            seq_val[seq_len++] = (seq_t)k;
        }
    }

    return seq_val[(size_t)n - 1];
}
