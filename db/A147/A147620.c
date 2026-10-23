/*
 * A147620  Expansion of g.f.: 1/((1 - x - x^2 + x^6 - x^8)*(1 - x^2 + x^6 +
 *          x^7 - x^8)).
 * https://oeis.org/A147620
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a recurrence derived from 1/((1 - x - x² + x⁶ - x⁸)(1 - x² + x⁶
 *            + x⁷ - x⁸)) with initial values a(0)=1, a(1)=1, a(2)=3, a(3)=4,
 *            a(4)=8, a(5)=12, a(6)=19, a(7)=29, a(8)=46, a(9)=70, a(10)=111,
 *            a(11)=170, a(12)=271, a(13)=422, a(14)=668, a(15)=1048.
 *            Recurrence (order 16) for n ≥ 16: a(n) = a(n-1)+2...
 * Range:     u8 n <= 11, u16 n <= 24, u32 n <= 48, u64 n <= 97, u128 n <= 195,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static buffer for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;          /* number of valid entries stored */
    static size_t cache_cap   = 0;          /* allocated capacity (in seq_t units) */

    /* if the requested index is already calculated, just return it */
    if ((size_t)n < cache_size)
        return cache[(size_t)n];

    /* ensure capacity for at least (n+1) entries */
    size_t needed = (size_t)n + 1;
    if (needed > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap < needed)
            new_cap *= 2;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache)
            abort();               /* out of memory – unlikely in the judge */
        if (cache) {
            memcpy(new_cache, cache, cache_size * sizeof(seq_t));
            free(cache);
        }
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* initial values for n = 0 … 15 */
    static const seq_t base[16] = {
        (seq_t)1, (seq_t)1, (seq_t)3, (seq_t)4,
        (seq_t)8, (seq_t)12,(seq_t)19,(seq_t)29,
        (seq_t)46,(seq_t)70,(seq_t)111,(seq_t)170,
        (seq_t)271,(seq_t)422,(seq_t)668,(seq_t)1048
    };

    /* compute values up to the requested index */
    for (size_t i = cache_size; i < needed; ++i) {
        if (i < 16) {
            cache[i] = base[i];
        } else {
            /* positive part of the recurrence: a(n‑1)+2a(n‑2)+5a(n‑8)+2a(n‑14)+a(n‑15) */
            seq_t pos = cache[i - 1];
            pos += 2 * cache[i - 2];
            pos += 5 * cache[i - 8];
            pos += 2 * cache[i - 14];
            pos += cache[i - 15];

            /* negative part: a(n‑3)+a(n‑4)+2a(n‑6)+2a(n‑10)+a(n‑12)+a(n‑13)+a(n‑16) */
            seq_t neg = cache[i - 3];
            neg += cache[i - 4];
            neg += 2 * cache[i - 6];
            neg += 2 * cache[i - 10];
            neg += cache[i - 12];
            neg += cache[i - 13];
            neg += cache[i - 16];

            cache[i] = pos - neg;    /* final value */
        }
    }
    cache_size = needed;
    return cache[(size_t)n];
}
