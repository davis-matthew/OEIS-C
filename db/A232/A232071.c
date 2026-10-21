/*
 * A232071  Number of (n+1)X(3+1) 0..1 arrays with every element equal to some
 *          horizontal, diagonal or antidiagonal neighbor, with top left
 *          element zero.
 * https://oeis.org/A232071
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*

OEIS formula:
a(n) = 10*a(n-1) + 33*a(n-2) +  6*a(n-3)
      -176*a(n-4) -300*a(n-5) - 15*a(n-6)
      - 76*a(n-7) + 79*a(n-8) + 72*a(n-9)
      - 16*a(n-10)
Initial values (n = 1 … 10):

1  46
2  520
3  6624
4  82996
5 1043172
6 13105012
7 164650280
8 2068621706
9 25989674166
10 326528021922

*/

#include <stdlib.h>

/* cache of values, indexed by (n-1) */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of computed values */
static size_t cache_cap = 0;   /* capacity of cache */

/* first 10 terms of the sequence */
static const seq_t init_vals[10] = {
    (seq_t)46,
    (seq_t)520,
    (seq_t)6624,
    (seq_t)82996,
    (seq_t)1043172,
    (seq_t)13105012,
    (seq_t)164650280,
    (seq_t)2068621706,
    (seq_t)25989674166,
    (seq_t)326528021922
};

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined, but harmless */

    /* Ensure capacity up to n */
    if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < (size_t)n) new_cap = (size_t)n;
        seq_t *buf = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!buf) exit(EXIT_FAILURE);   /* allocation failure */
        cache = buf;
        cache_cap = new_cap;
    }

    /* fill original 10 values if missing */
    if (cache_len < 10) {
        size_t i = cache_len;
        size_t limit = 10;
        if ((size_t)n < 10) limit = (size_t)n;
        for (; i < limit; ++i) cache[i] = init_vals[i];
        cache_len = limit;
    }

    /* compute further terms */
    while (cache_len < (size_t)n) {
        size_t i = cache_len;               /* zero‑based index (a(i+1)) */
        /* for i < 10 we already have values */
        if (i < 10) { ++cache_len; continue; }

        /* recurrence using only unsigned arithmetic; the value is always
           non‑negative, so subtraction will not underflow.               */
        seq_t res = ((seq_t)10   * cache[i-1]) +
                    ((seq_t)33   * cache[i-2]) +
                    ((seq_t)6    * cache[i-3]) +
                    ((seq_t)79   * cache[i-8]) +
                    ((seq_t)72   * cache[i-9]) -
                    ((seq_t)176  * cache[i-4]) -
                    ((seq_t)300  * cache[i-5]) -
                    ((seq_t)15   * cache[i-6]) -
                    ((seq_t)76   * cache[i-7]) -
                    ((seq_t)16   * cache[i-10]);

        cache[i] = res;
        ++cache_len;
    }

    return cache[(size_t)n - 1];
}
