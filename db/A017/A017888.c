/*
 * A017888  Expansion of 1/(1 - x^10 - x^11 - x^12).
 * https://oeis.org/A017888
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-10) + a(n-11) + a(n-12) for n > 11; a(0)=1;
 *            a(1..9)=0; a(10)=a(11)=a(12)=1;
 * Range:     u8 n <= 74, u16 n <= 130, u32 n <= 246, u64 n <= 466,
 *            u128 n <= 909, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_cap = 0;
static idx_t cache_len = 0;

seq_t a(idx_t n) {
    if (n < cache_len)
        return cache[n];

    if (n + 1 > cache_cap) {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            return 0; /* allocation failure */
        for (idx_t i = cache_cap; i < new_cap; ++i)
            tmp[i] = 0;
        cache = tmp;
        cache_cap = new_cap;
    }

    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val;
        if (i == 0)
            val = 1;
        else if (i < 10)
            val = 0;
        else if (i == 10 || i == 11 || i == 12)
            val = 1;
        else
            val = cache[i-10] + cache[i-11] + cache[i-12];
        cache[i] = val;
    }
    cache_len = n + 1;
    return cache[n];
}
