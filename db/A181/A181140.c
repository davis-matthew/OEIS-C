/*
 * A181140  The number of ways to color n balls in a row with 3 colors with no
 *          color runs having lengths greater than 4. This sequence is a
 *          special case of the general problem for coloring n balls in a row
 *          with p colors where each color has a given maximum run-length. In
 *          this example, the bounds are uniforml...
 * https://oeis.org/A181140
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A181140: number of colorings of n balls with 3 colors, forbidding runs longer than 4.
   Recurrence: a(n+4) = 2*(a(n)+a(n+1)+a(n+2)+a(n+3)), n ≥ 1, with
   a(1)=3, a(2)=9, a(3)=27, a(4)=81.  The sequence is computed on demand with
   memoization; any order of calls is supported. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Special cases for the first 4 terms. */
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)9;
    if (n == 3) return (seq_t)27;
    if (n == 4) return (seq_t)81;

    /* Static cache for computed terms. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Allocate and fill initial 4 terms only once. */
    if (cache == NULL) {
        cache_size = 4;
        cache = (seq_t *)malloc(4 * sizeof(seq_t));
        cache[0] = (seq_t)3;
        cache[1] = (seq_t)9;
        cache[2] = (seq_t)27;
        cache[3] = (seq_t)81;
    }

    /* Extend the cache up to the requested n. */
    if (n > cache_size) {
        seq_t *tmp = (seq_t *)realloc(cache, n * sizeof(seq_t));
        if (tmp == NULL) {
            /* Allocation failed – in production code we might set errno
               or handle the error, but for this CTF-style function we
               simply abort. */
            abort();
        }
        cache = tmp;

        /* Compute missing terms using the recurrence. */
        for (idx_t i = cache_size; i < n; ++i) {
            /* i is zero‑based index; a(i+1) refers to cache[i] */
            seq_t sum = cache[i-1] + cache[i-2] + cache[i-3] + cache[i-4];
            cache[i] = (seq_t)2 * sum;
        }
        cache_size = n;
    }

    return cache[n-1];
}
