/*
 * A036228  a(1) = 31; a(n+1) = a(n) + sum of decimal digits of a(n).
 * https://oeis.org/A036228
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 31; a(n+1) = a(n) + digit_sum(a(n)) */
#include <stdlib.h>
#include "../utils/digit_sum.h"

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t len  = 0;
    static idx_t cap  = 0;

    if (cache == NULL) {
        cap = 1;
        cache = (seq_t*)malloc(sizeof(seq_t) * cap);
        if (!cache) return (seq_t)0;   /* allocation failed */
    } else if (cap < n) {
        idx_t newcap = cap;
        if (newcap == 0) newcap = 1;
        while (newcap < n) newcap <<= 1;   /* grow by powers of two */
        seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * newcap);
        if (!tmp) return (seq_t)0;   /* reallocation failed */
        cache = tmp;
        cap = newcap;
    }

    while (len < n) {
        if (len == 0) {
            cache[0] = (seq_t)31;
        } else {
            seq_t last = cache[len - 1];
            seq_t ds   = digit_sum(last);
            cache[len] = last + ds;
        }
        ++len;
    }
    return cache[n - 1];
}
