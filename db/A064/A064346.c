/*
 * A064346  Generalized Catalan numbers C(8,8; n).
 * https://oeis.org/A064346
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Recurrence used by OEIS A064346:
      7*(-n+1)*a(n) + 8*(223*n-560)*a(n-1) + 1024*(2*n-3)*a(n-2) = 0
   Initial values:  a(0)=1, a(1)=1, a(2)=16
*/

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* initialise cache with first three terms */
    if (cache_len < 3) {
        size_t need = 3;
        seq_t *tmp = (seq_t *)realloc(cache, need * sizeof(seq_t));
        if (!tmp) return (seq_t)0;           /* allocation failed */
        cache = tmp;
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)16;
        cache_len = 3;
    }

    size_t target = (size_t)n;
    if (target + 1 > cache_len) {
        size_t new_len = target + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        cache = tmp;

        for (size_t i = cache_len; i <= target; ++i) {
            if (i == 0 || i == 1 || i == 2) {
                /* already initialised */
                continue;
            }

            idx_t ii = (idx_t)i;

            /* use 128‑bit intermediate to avoid overflow on 64‑bit builds */
            unsigned __int128 coeff1 = (unsigned __int128)8 * (223 * ii - 560);
            unsigned __int128 coeff2 = (unsigned __int128)1024 * (2 * ii - 3);
            unsigned __int128 denom  = (unsigned __int128)7 * (ii - 1);

            unsigned __int128 term1 = coeff1 * (unsigned __int128)cache[i - 1];
            unsigned __int128 term2 = coeff2 * (unsigned __int128)cache[i - 2];
            unsigned __int128 num   = term1 + term2;

            cache[i] = (seq_t)(num / denom);
        }
        cache_len = new_len;
    }

    return cache[target];
}
