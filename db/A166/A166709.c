/*
 * A166709  Number of distinct integers expressible as sums of consecutive
 *          primes up to n-th prime.
 * https://oeis.org/A166709
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 381, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = number of distinct sums of consecutive primes up to the n‑th prime */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/first_n_primes.h"

/* ------------------------------------------------------------------ */
/* Helper: generate the first n primes, returning a newly allocated   */
/* array of size n.  The caller must free the returned array.          */

/* ------------------------------------------------------------------ */
/* Main sequence function                                             */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;   /* offset is 1 */

    /* Get first n primes */
    idx_t *primes = first_n_primes(n);
    if (!primes) return (seq_t)0;

    /* Cumulative sums of primes (uint64_t to avoid overflow). */
    uint64_t *cum = (uint64_t *)calloc((size_t)n + 1, sizeof(uint64_t));
    if (!cum) { free(primes); return (seq_t)0; }
    for (idx_t i = 0; i < n; ++i)
        cum[(size_t)i + 1] = cum[(size_t)i] + (uint64_t)primes[i];

    uint64_t maxSum = cum[(size_t)n];

    /* Bitset size: number of 64‑bit words needed to cover 0..maxSum */
    size_t bits  = (size_t)maxSum + 1;
    size_t words = (bits >> 6) + 1;

    uint64_t *bitset = (uint64_t *)calloc(words, sizeof(uint64_t));
    if (!bitset) { free(cum); free(primes); return (seq_t)0; }

    /* Mark every sum of a consecutive prime interval */
    for (idx_t i = 0; i < n; ++i) {
        for (idx_t j = i; j < n; ++j) {
            uint64_t sum = cum[(size_t)j + 1] - cum[(size_t)i];
            size_t idx   = (size_t)sum;
            bitset[idx >> 6] |= ((uint64_t)1 << (idx & 63));
        }
    }

    /* Count set bits (distinct sums) */
    seq_t count = 0;
    for (size_t w = 0; w < words; ++w)
        count += (seq_t)__builtin_popcountll(bitset[w]);

    /* Free allocations */
    free(bitset);
    free(cum);
    free(primes);

    return count;
}
