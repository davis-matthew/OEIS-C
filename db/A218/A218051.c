/*
 * A218051  Number of n X 1 arrays of the minimum value of corresponding
 *          elements and their horizontal, vertical, diagonal or antidiagonal
 *          neighbors in a random 0..3 n X 1 array.
 * https://oeis.org/A218051
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + a(n-2) + 2*a(n-4) + a(n-5) (n >= 6) Initial
 *            values: a(1)=4, a(2)=4, a(3)=16, a(4)=36, a(5)=94
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 47, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute the nth term of OEIS sequence A218051.
 * The sequence is defined by the recurrence:
 *   a(1)=4, a(2)=4, a(3)=16, a(4)=36, a(5)=94,
 *   a(n)=2*a(n-1)+a(n-2)+2*a(n-4)+a(n-5)  for n>=6.
 *
 * The function keeps a static cache so that subsequent calls are fast
 * even if the arguments are out of order.
 */
seq_t a(idx_t n)
{
    /* static state */
    static seq_t *cache = NULL;       /* linear cache of values */
    static size_t cache_cap = 0;      /* allocated capacity of cache */
    static idx_t last_computed = 0;   /* highest n that has been computed */

    /* For n < 1 the offset is not defined; return 0. */
    if (n < 1) {
        return (seq_t)0;
    }

    /* Ensure cache can hold indices up to n (including 0). */
    size_t need = (size_t)n + 1;
    if (cache_cap < need) {
        /* reallocate: if cache is NULL, allocate anew; otherwise grow */
        seq_t *tmp = (cache == NULL) ? (seq_t*)malloc(need * sizeof(seq_t))
                                     : (seq_t*)realloc(cache, need * sizeof(seq_t));
        if (tmp == NULL) {
            /* allocation failure – abort or return 0 */
            return (seq_t)0;
        }
        cache = tmp;
        cache_cap = need;
    }

    /* Compute any missing values up to n. */
    for (size_t i = (size_t)(last_computed + 1); i <= (size_t)n; ++i) {
        if (i <= 5) {
            /* Base values */
            switch ((int)i) {
                case 1: cache[i] = (seq_t)4; break;
                case 2: cache[i] = (seq_t)4; break;
                case 3: cache[i] = (seq_t)16; break;
                case 4: cache[i] = (seq_t)36; break;
                case 5: cache[i] = (seq_t)94; break;
                default: break; /* never reached */
            }
        } else {
            /* Recurrence: a(n)=2*a(n-1)+a(n-2)+2*a(n-4)+a(n-5) */
            seq_t val = cache[i - 1];
            val += cache[i - 1];          /* 2*a(i-1) */
            val += cache[i - 2];          /* + a(i-2) */
            val += cache[i - 4];
            val += cache[i - 4];          /* + 2*a(i-4) */
            val += cache[i - 5];          /* + a(i-5) */
            cache[i] = val;
        }
    }

    last_computed = (idx_t)n;
    return cache[n];
}
