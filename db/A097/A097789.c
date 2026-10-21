/*
 * A097789  a(n)=4a(n-1)+C(n+4,4),n>0, a(0)=1.
 * https://oeis.org/A097789
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4a(n-1)+C(n+4,4), a(0)=1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* cached values */
static size_t cache_size = 0;    /* number of initialized entries */
static size_t cache_cap   = 0;    /* allocated capacity */

seq_t a(idx_t n)
{
    size_t idx = (size_t)n;
    if (idx >= cache_size)
    {
        size_t need = idx + 1;
        if (need > cache_cap)
        {
            size_t new_cap = cache_cap ? cache_cap : 1;
            while (new_cap < need) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cache_cap = new_cap;
        }

        for (size_t i = cache_size; i <= idx; ++i)
        {
            if (i == 0)
            {
                cache[0] = (seq_t)1;
            }
            else
            {
                /* C(i+4,4) = (i+4)(i+3)(i+2)(i+1)/24 */
                seq_t bin = ((seq_t)(i + 4) * (i + 3) * (i + 2) * (i + 1)) / 24;
                cache[i] = 4 * cache[i - 1] + bin;
            }
        }
        cache_size = need;
    }
    return cache[idx];
}
