/*
 * A027711  Number of binary sequences of length n with an even number of
 *          ones, at least two of the ones being contiguous.
 * https://oeis.org/A027711
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 5*a(n-2) + 2*a(n-3) + a(n-4) - 2*a(n-5)
 *            Initial values: a(1)=0, a(2)=1, a(3)=2, a(4)=4, a(5)=9 The
 *            function memoizes results so it works for arbitrary call order
 *            and any n ≥ 1
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc */

/* static cache for memoization */
static seq_t *cache = NULL;          /* array of computed a(n) values, 1‑indexed */
static idx_t  cached_max = 0;        /* highest n that has been computed */

/* helper to compute the sequence up to a given n */
static void grow_cache_to(idx_t n)
{
    if (n <= cached_max) return;           /* already computed */

    /* Allocate initial elements if necessary */
    if (cache == NULL) {
        /* indices 0..5, we ignore index 0 */
        cache = (seq_t *)malloc((6) * sizeof(seq_t));
        /* initial values */
        cache[0] = 0; /* unused */
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)2;
        cache[4] = (seq_t)4;
        cache[5] = (seq_t)9;
        cached_max = 5;
    }

    /* grow array to hold up to n */
    if (n > cached_max) {
        cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        /* compute new values */
        for (idx_t i = cached_max + 1; i <= n; ++i) {
            /* recurrence uses a(i-1) … a(i-5) */
            seq_t a1 = cache[i - 1];
            seq_t a2 = cache[i - 2];
            seq_t a3 = cache[i - 3];
            seq_t a4 = cache[i - 4];
            seq_t a5 = cache[i - 5];
            seq_t val = 4 * a1 - 5 * a2 + 2 * a3 + a4 - 2 * a5;
            cache[i] = val;
        }
        cached_max = n;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* not defined below offset, return 0 */
    grow_cache_to(n);
    return cache[n];
}
