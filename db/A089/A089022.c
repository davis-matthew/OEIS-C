/*
 * A089022  Number of walks of length 2n on the 3-regular tree beginning and
 *          ending at some fixed vertex.
 * https://oeis.org/A089022
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ( (17*n-12)*a(n-1) - 36*(2*n-3)*a(n-2) ) / n, with
 *            a(0)=1, a(1)=3. This recurrence follows the D‑finite relation
 *            n*a(n) + (12-17*n)*a(n-1) + 36*(2n-3)*a(n-2) = 0
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;   /* cached values of a(n)          */
static size_t cache_cap = 0;  /* allocated capacity             */
static size_t cache_len = 0;  /* highest cached index + 1       */

seq_t a(idx_t n)
{
    /* initialise cache with the first two values */
    if (cache == NULL) {
        cache_cap = 2;
        cache_len = 2;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) exit(1);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
    }

    /* grow cache if necessary */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap < (size_t)n + 1) new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) exit(1);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute values up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {            /* already stored */
            cache[i] = (seq_t)1;
        } else if (i == 1) {      /* already stored */
            cache[i] = (seq_t)3;
        } else {
            /* Use a larger intermediate type when seq_t is small,
               otherwise rely on seq_t (big integer) arithmetic. */
            if (sizeof(seq_t) <= 8) {
#if defined(__SIZEOF_INT128__)
                __int128 term1 = (__int128)cache[i-1] * (__int128)(17 * i - 12);
                __int128 term2 = (__int128)cache[i-2] * (__int128)36
                                * (__int128)(2 * i - 3);
                __int128 num   = term1 - term2;
                cache[i] = (seq_t)(num / (seq_t)i);
#else
                unsigned long long term1 = (unsigned long long)cache[i-1]
                  * (unsigned long long)(17 * i - 12);
                unsigned long long term2 = (unsigned long long)cache[i-2]
                  * 36ULL * (unsigned long long)(2 * i - 3);
                unsigned long long num   = term1 - term2;
                cache[i] = (seq_t)(num / (unsigned long long)i);
#endif
            } else {
                /* seq_t is at least 16 bytes (e.g. big‑integer type). */
                seq_t term1 = cache[i-1] * (seq_t)(17 * i - 12);
                seq_t term2 = cache[i-2] * (seq_t)36 * (seq_t)(2 * i - 3);
                seq_t num   = term1 - term2;
                cache[i] = num / (seq_t)i;
            }
        }
    }
    cache_len = (size_t)n + 1;
    return cache[n];
}
