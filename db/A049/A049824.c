/*
 * A049824  a(n) = number of 3's in n-th row of array T given by A049816.
 * https://oeis.org/A049824
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function implements sequence A049824: */
/* a(n) = number of k (1 ≤ k ≤ n) for which the Euclidean algorithm
   on the pair (n, k) has exactly three non‑zero remainders.
   The implementation lazily computes a(n) on demand, caching
   all previously computed values.  It uses an O(n²) precomputation
   approach which is fast enough for n ≤ 10 000. */

#include <stdlib.h>

static seq_t *cache = NULL;          /* array of computed a(n) values */
static idx_t cached_max = 0;         /* largest n computed so far */

/* Compute a(i) for all i > cached_max up to 'limit',
   storing the results in the cache.  The cache is grown
   as necessary. */
static void compute_upto(idx_t limit)
{
    if (limit <= cached_max) return;

    /* Allocate or resize the cache array. */
    size_t newsize = (size_t)(limit) + 1; /* indices 0..limit inclusive */
    if (cache == NULL) {
        cache = (seq_t *)malloc(newsize * sizeof(seq_t));
    } else {
        cache = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
    }

    /* Safety: set unused element 0. */
    cache[0] = (seq_t)0;

    /* Compute values for all new indices. */
    for (idx_t i = cached_max + 1; i <= limit; ++i) {
        seq_t count = (seq_t)0;  /* number of k with exactly 3 steps */

        for (idx_t k = 1; k <= i; ++k) {
            seq_t a_val = (seq_t)i;
            seq_t b_val = (seq_t)k;
            seq_t steps = (seq_t)0;

            /* Euclidean algorithm counting non‑zero remainders. */
            while (1) {
                seq_t r = a_val % b_val;
                if (r == (seq_t)0) break;
                ++steps;
                if (steps > (seq_t)3) break;  /* early exit */
                a_val = b_val;
                b_val = r;
            }

            if (steps == (seq_t)3) ++count;
        }

        cache[i] = count;
    }

    cached_max = limit;
}

/* Public function required by the problem statement. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;     /* offset is 1; undefined below 1 */
    compute_upto(n);
    return cache[n];
}
