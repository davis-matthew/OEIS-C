/*
 * A203446  Number of (n+1) X 3 0..1 arrays with column and row pair sums
 *          b(i,j)=a(i,j)+a(i,j-1) and c(i,j)=a(i,j)+a(i-1,j) such that rows
 *          of b(i,j) and columns of c(i,j) are lexicographically
 *          nondecreasing.
 * https://oeis.org/A203446
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 4*a(n-2) - 2*a(n-3) + 5*a( n-4) - 2*a( n-5),
 *            initial values: a(1)=16, a(2)=34, a(3)=80, a(4)=174, a(5)=376
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 28, u64 n <= 60, u128 n <= 124,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static memoisation table */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t cached_upto = 0;  /* highest n with computed value */

    /* ensure cache large enough (index 0 unused) */
    if (cap < (size_t)(n + 1)) {
        size_t new_cap = cap ? cap * 2 : 64;
        if (new_cap < (size_t)(n + 1))
            new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) { /* allocation failure */
            exit(EXIT_FAILURE);
        }
        cache = tmp;
        cap = new_cap;
    }

    /* compute missing values */
    for (idx_t i = cached_upto + 1; i <= n; ++i) {
        if (i == 1)      cache[i] = (seq_t)16;
        else if (i == 2) cache[i] = (seq_t)34;
        else if (i == 3) cache[i] = (seq_t)80;
        else if (i == 4) cache[i] = (seq_t)174;
        else if (i == 5) cache[i] = (seq_t)376;
        else {
            seq_t left  = cache[i-1] * (seq_t)4 + cache[i-4] * (seq_t)5;
            seq_t right = cache[i-2] * (seq_t)4 + cache[i-3] * (seq_t)2
                        + cache[i-5] * (seq_t)2;
            cache[i] = left - right;
        }
    }
    cached_upto = n;
    return cache[n];
}
