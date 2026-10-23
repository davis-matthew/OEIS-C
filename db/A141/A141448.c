/*
 * A141448  Generalized Pell numbers P(n,5,5).
 * https://oeis.org/A141448
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1)+a(n-2)+a(n-3)+a(n-4)+a(n-5), a(0)=0, a(1)=1,
 *            a(2)=2, a(3)=5, a(4)=13
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 24, u64 n <= 47, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t len = 0;
    static bool init = false;

    size_t needed = (size_t)n + 1;

    /* ensure storage capacity */
    if (needed > cap) {
        size_t new_cap = cap ? cap : 1;
        while (new_cap < needed) new_cap <<= 1;
        if (new_cap < 5) new_cap = 5;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* fallback on allocation failure */
        cache = tmp;
        cap = new_cap;
    }

    /* one‑time initialization of the first five terms */
    if (!init) {
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)5;
        cache[4] = (seq_t)13;
        len = 5;
        init = true;
    }

    /* extend the sequence up to n */
    if (needed > len) {
        for (size_t i = len; i < needed; ++i) {
            cache[i] =
                (seq_t)2 * cache[i-1] +
                cache[i-2] +
                cache[i-3] +
                cache[i-4] +
                cache[i-5];
        }
        len = needed;
    }

    return cache[n];
}
