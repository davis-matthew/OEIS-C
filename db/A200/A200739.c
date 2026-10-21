/*
 * A200739  Expansion of (-x^2+5*x-1)/(x^3-x^2+5*x-1).
 * https://oeis.org/A200739
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1,a(1)=0,a(2)=0; for n≥3: a(n)=a(n‑3)+5*a(n‑1)−a(n‑2)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 17, u64 n <= 31, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

seq_t a(idx_t n)
{
    size_t idx = (size_t)n;

    if (idx >= cache_len) {
        /* adjust storage if needed */
        size_t need = idx + 1;
        if (cache == NULL) {
            cache_cap = need > 16 ? need : 16;
            cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        } else if (need > cache_cap) {
            size_t new_cap = cache_cap << 1;
            if (new_cap < need) new_cap = need;
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache_cap = new_cap;
        }

        /* fill all missing terms */
        for (size_t i = cache_len; i <= idx; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)1;
            } else if (i == 1 || i == 2) {
                cache[i] = (seq_t)0;
            } else {
                seq_t val = cache[i-3];
                val += cache[i-1] * (seq_t)5;
                val -= cache[i-2];
                cache[i] = val;
            }
        }
        cache_len = need;
    }

    return cache[idx];
}
