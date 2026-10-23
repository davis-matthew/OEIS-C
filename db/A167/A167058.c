/*
 * A167058  Number of spanning trees in (S_5 + e) X P_n.
 * https://oeis.org/A167058
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 270 a(n-1) - 10529 a(n-2) + 95310 a(n-3) - 177156 a(n-4)
 *            + 95310 a(n-5) - 10529 a(n-6) + 270 a(n-7) - a(n-8) with a(1)=3,
 *            a(2)=945, a(3)=221184, a(4)=50055705, a(5)=11275732875,
 *            a(6)=2538325278720, a(7)=571357349020731,
 *            a(8)=128606300878893705
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    /* Static cache of already computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static size_t cache_cap = 0;

    /* First 8 terms (known from the sequence definition) */
    static const seq_t init[8] = {
        (seq_t)3,
        (seq_t)945,
        (seq_t)221184,
        (seq_t)50055705,
        (seq_t)11275732875ULL,
        (seq_t)2538325278720ULL,
        (seq_t)571357349020731ULL,
        (seq_t)128606300878893705ULL
    };

    /* First time: allocate cache and store the 8 initial terms */
    if (cache == NULL) {
        cache_cap = 16;                     /* initial capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL)
            return (seq_t)0;                /* allocation failure – return 0 */
        for (idx_t i = 0; i < 8; ++i) {
            cache[i] = init[i];
        }
        cache_len = 8;
    }

    /* If already computed, return quickly */
    if (n <= cache_len)
        return cache[n - 1];

    /* Compute terms up to n (inclusive) */
    for (idx_t m = cache_len; m < n; ++m) {
        /* recurrence uses the previous eight terms */
        seq_t val = cache[m - 1] * (seq_t)270;
        val -= cache[m - 2] * (seq_t)10529;
        val += cache[m - 3] * (seq_t)95310;
        val -= cache[m - 4] * (seq_t)177156;
        val += cache[m - 5] * (seq_t)95310;
        val -= cache[m - 6] * (seq_t)10529;
        val += cache[m - 7] * (seq_t)270;
        val -= cache[m - 8];

        /* Grow cache if necessary */
        if (cache_len == cache_cap) {
            cache_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            if (tmp == NULL)
                return (seq_t)0;            /* allocation failure – return 0 */
            cache = tmp;
        }

        cache[cache_len++] = val;
    }

    return cache[n - 1];
}
