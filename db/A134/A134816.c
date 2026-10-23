/*
 * A134816  Padovan's spiral numbers.
 * https://oeis.org/A134816
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-2) + a(n-3) for n>3, with a(1)=a(2)=a(3)=1
 * Range:     u8 n <= 21, u16 n <= 41, u32 n <= 81, u64 n <= 159,
 *            u128 n <= 317, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n < 1) return 0;

    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap = 0;
    size_t nn = (size_t)n;

    /* initialize first three terms */
    if (cache_size < 3) {
        size_t needed = 3;
        if (needed > cache_cap) {
            seq_t *tmp = (seq_t*)realloc(cache, needed * sizeof(seq_t));
            if (!tmp) return 0;          /* allocation failed */
            cache = tmp;
            cache_cap = needed;
        }
        cache[0] = 1; /* a(1) */
        cache[1] = 1; /* a(2) */
        cache[2] = 1; /* a(3) */
        cache_size = 3;
    }

    /* grow cache if needed */
    if (nn > cache_cap) {
        size_t new_cap = cache_cap == 0 ? 4 : cache_cap;
        while (new_cap < nn) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;              /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms */
    for (size_t i = cache_size; i < nn; ++i) {
        cache[i] = cache[i-2] + cache[i-3];
    }
    cache_size = nn;

    return cache[nn-1];
}
