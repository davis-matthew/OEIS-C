/*
 * A116732  a(n) = a(n-1) + a(n-2) + a(n-3) - a(n-4) with a(0) = a(1) = a(2) =
 *          0, a(3) = 1.
 * https://oeis.org/A116732
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 44, u64 n <= 85, u128 n <= 166,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = a(n-1) + a(n-2) + a(n-3) - a(n-4)
 * initial values: a(0)=0, a(1)=0, a(2)=0, a(3)=1
 */

#include <stdlib.h>

/* Computes A116732 a(n).  Uses a static cache so calls may come in any order. */
seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;          /* array of computed terms */
    static idx_t cache_len = 0;          /* number of terms stored */

    if (n >= cache_len) {
        /* need to extend the cache up to n */
        idx_t new_len = n + 1;
        /* realloc is fine in both C and C++ (C‑style cast is allowed in C++) */
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) {
            /* allocation failed – fall back to iterative computation for this call */
            cache = NULL;
            cache_len = 0;
        } else {
            cache = tmp;
            /* initialise new entries */
            for (idx_t i = cache_len; i < new_len; ++i) {
                if (i == 0 || i == 1 || i == 2) {
                    cache[i] = (seq_t)0;
                } else if (i == 3) {
                    cache[i] = (seq_t)1;
                } else {
                    /* recurrence uses previously computed terms */
                    cache[i] = cache[i-1] + cache[i-2] + cache[i-3] - cache[i-4];
                }
            }
            cache_len = new_len;
        }
    }

    if (cache && n < cache_len) {
        /* cached value available */
        return cache[n];
    }

    /* allocation failed – compute directly without caching */
    seq_t res;
    if (n == 0 || n == 1 || n == 2) {
        res = (seq_t)0;
    } else if (n == 3) {
        res = (seq_t)1;
    } else {
        seq_t a0 = 0, a1 = 0, a2 = 0, a3 = 1, ai;
        for (idx_t i = 4; i <= n; ++i) {
            ai = a3 + a2 + a1 - a0;
            a0 = a1;
            a1 = a2;
            a2 = a3;
            a3 = ai;
        }
        res = a3;
    }
    return res;
}
