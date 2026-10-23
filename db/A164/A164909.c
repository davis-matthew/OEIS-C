/*
 * A164909  Number of reduced words of length n in Coxeter group on 19
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^8 = I.
 * https://oeis.org/A164909
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 17·(a(n-1)+...+a(n-7)) – 153·a(n-8)  for n ≥ 9
   Initial values: a(0)=1, a(1)=19, a(2)=342, a(3)=6156, a(4)=110808,
   a(5)=1994544, a(6)=35901792, a(7)=646232256, a(8)=11632180437 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Base values for indices 0 … 8. */
    static const seq_t base_values[9] = {
        (seq_t)1ULL,
        (seq_t)19ULL,
        (seq_t)342ULL,
        (seq_t)6156ULL,
        (seq_t)110808ULL,
        (seq_t)1994544ULL,
        (seq_t)35901792ULL,
        (seq_t)646232256ULL,
        (seq_t)11632180437ULL
    };

    /* Cache of already computed terms. */
    static seq_t *cache   = NULL;   /* cache[0 .. cache_len-1] = a(0) … a(cache_len-1) */
    static size_t cache_len = 0;     /* number of elements in cache */

    /* Need to have terms 0 … n. */
    size_t need = (size_t)n + 1;

    if (need > cache_len) {
        /* Grow the cache.  realloc returns void*, cast to seq_t* for C++ */
        size_t old_len = cache_len;
        cache = (seq_t *)realloc(cache, need * sizeof(seq_t));
        cache_len = need;

        /* Initialise new entries. */
        for (size_t i = old_len; i < need; ++i) {
            if (i <= 8) {
                /* Initial values. */
                cache[i] = base_values[i];
            } else {
                /* Recurrence for i ≥ 9. */
                seq_t sum = 0;
                for (int k = 1; k <= 7; ++k) {
                    sum += cache[i - k];
                }
                cache[i] = (seq_t)17 * sum - (seq_t)153 * cache[i - 8];
            }
        }
    }

    return cache[(size_t)n];
}
