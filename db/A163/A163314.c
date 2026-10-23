/*
 * A163314  Number of reduced words of length n in Coxeter group on 3
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^5 = I.
 * https://oeis.org/A163314
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + 2*a(n-3) - a(n-4); Initial values:
 *            a(0)=1, a(1)=3, a(2)=6, a(3)=12, a(4)=24
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 34, u64 n <= 69, u128 n <= 139,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;     /* memoised values             */
static size_t cap   = 0;        /* allocated array size        */
static idx_t top    = (idx_t)(-1);/* highest index computed */

seq_t a(idx_t n)
{
    /*--- initialise cache with the first five terms ---*/
    if (top == (idx_t)(-1)) {
        cap = 5;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;        /* allocation failed */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)6;
        cache[3] = (seq_t)12;
        cache[4] = (seq_t)24;
        top    = 4;
    }

    /*--- if the requested value is already cached ---*/
    if (n <= top) return cache[n];

    /*--- grow the cache so that index n can be stored ---*/
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap ? cap * 2 : 8;
        while (new_cap <= (size_t)n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;             /* allocation failed */
        cache = tmp;
        cap   = new_cap;
    }

    /*--- compute terms from top+1 up to n ---*/
    for (size_t i = (size_t)(top + 1); i <= (size_t)n; ++i) {
        seq_t v1 = cache[i-1];
        seq_t v2 = cache[i-2];
        seq_t v3 = cache[i-3];
        seq_t v4 = cache[i-4];

        /* a(i) = 2*a(i-1) - a(i-2) + 2*a(i-3) - a(i-4) */
        seq_t val = v1 + v1;   /* 2*a(i-1) */
        val = val - v2;        /* -a(i-2) */
        val = val + v3 + v3;   /* +2*a(i-3) */
        val = val - v4;        /* -a(i-4) */
        cache[i] = val;
    }

    top = n;
    return cache[n];
}
