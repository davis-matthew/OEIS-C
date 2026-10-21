/*
 * A208874  Number of 3Xn 0..3 arrays with new values 0..3 introduced in row
 *          major order and no element equal to more than one of its immediate
 *          leftward or upward or right-upward antidiagonal neighbors.
 * https://oeis.org/A208874
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 61*a(n-1) - 722*a(n-2) + 3476*a(n-3) - 7851*a(n-4) +
 *            8583*a(n-5) - 4266*a(n-6) + 720*a(n-7) for n>8 Initial values:
 *            a(1)=5, a(2)=143, a(3)=6520, a(4)=307165, a(5)=14493405,
 *            a(6)=683903931, a(7)=32271626438, a(8)=1522813452263 (indices
 *            start at 1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Static cache of computed values, 1‑indexed. */
static seq_t *cache = NULL;
static idx_t cached_upto = 0;

/* Initialise cache with the first eight terms. */
static void init_cache(void)
{
    if (cache != NULL)
        return;

    /* 0 unused; indices 1..8 filled */
    cache = (seq_t*)calloc(9, sizeof(seq_t));
    cache[1] = (seq_t)5ULL;
    cache[2] = (seq_t)143ULL;
    cache[3] = (seq_t)6520ULL;
    cache[4] = (seq_t)307165ULL;
    cache[5] = (seq_t)14493405ULL;
    cache[6] = (seq_t)683903931ULL;
    cache[7] = (seq_t)32271626438ULL;
    cache[8] = (seq_t)1522813452263ULL;
    cached_upto = 8;
}

/* Return the n-th term of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;            /* undefined for offset 1 */

    init_cache();

    if (n > cached_upto) {
        /* Extend the allocation to accommodate up to n. */
        cache = (seq_t*)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));

        /* Compute terms from cached_upto+1 up to n. */
        for (idx_t k = cached_upto + 1; k <= n; ++k) {
            cache[k] =
                (seq_t)61   * cache[k-1]
              - (seq_t)722  * cache[k-2]
              + (seq_t)3476 * cache[k-3]
              - (seq_t)7851 * cache[k-4]
              + (seq_t)8583 * cache[k-5]
              - (seq_t)4266 * cache[k-6]
              + (seq_t)720  * cache[k-7];
        }
        cached_upto = n;
    }

    return cache[n];
}
