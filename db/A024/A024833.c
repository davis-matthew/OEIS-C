/*
 * A024833  a(n) = least m such that if r and s in {1/1, 1/2, 1/3, ..., 1/n}
 *          satisfy r < s, then r < k/m < (k+1)/m < s for some integer k.
 * https://oeis.org/A024833
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 190, u32 n <= 300, u64 n <= 300,
 *            u128 n <= 300, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A024833: least denominator m such that for each j=1..n-1
   there is an integer k with 1/(j+1) < k/m < (k+1)/m < 1/j. */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* aux: test whether m satisfies all constraints for given n */
static bool satisfies(idx_t n, seq_t m)
{
    /* iterate j from n-1 down to 1, stop early if any fails */
    for (idx_t j = n - 1; j > 0; --j) {
        seq_t L = m / (j + 1);        /* floor(m/(j+1)) */
        seq_t Q = m / j;              /* floor(m/j)       */
        seq_t d = (m % j == 0) ? 2 : 1;
        /* inequality: L + 1 + d <= Q  */
        if (L + 1 + d > Q)
            return false;
    }
    return true;
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* trivial handling for n < 2 (offset 2), behaviour irrelevant */
    if (n < 2)
        return 0;

    static seq_t *cache = NULL;      /* cache of previously computed terms */
    static idx_t max_cached = 1;     /* largest index for which cache is filled */

    /* initialise cache on first call */
    if (cache == NULL) {
        cache = (seq_t *)malloc(3 * sizeof(seq_t)); /* indices 0,1,2 */
        cache[0] = 0;
        cache[1] = 0;
        cache[2] = 5;                 /* a(2) = 5 */
        max_cached = 2;
    }

    /* extend cache to hold up to index n */
    if (n > max_cached) {
        cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        for (idx_t i = max_cached + 1; i <= n; ++i) {
            seq_t m = cache[i - 1];   /* start just above previous term */
            while (!satisfies(i, m))
                ++m;
            cache[i] = m;
        }
        max_cached = n;
    }

    return cache[n];
}
