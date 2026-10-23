/*
 * A133577  a(n) = a(n-1) + 10*a(n-2) for n >= 2, a(0)=1, a(1)=2.
 * https://oeis.org/A133577
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + 10*a(n-2); a(0)=1; a(1)=2
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t last = 0;

    if (n == 0) return 1;
    if (n == 1) return 2;

    if (cache == NULL) {
        size_t init = (n < 2 ? 2 : n + 1);
        cap = init;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;
        cache[0] = 1;
        cache[1] = 2;
        last = 1;
    }

    if (n > last) {
        size_t need = (size_t)n + 1;
        if (need > cap) {
            size_t newcap = cap ? cap * 2 : 16;
            if (newcap < need) newcap = need;
            seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
            if (tmp) {
                cache = tmp;
                cap = newcap;
            }
        }
        for (size_t i = last + 1; i <= (size_t)n; ++i) {
            cache[i] = cache[i - 1] + (seq_t)10 * cache[i - 2];
        }
        last = (size_t)n;
    }

    return cache[(size_t)n];
}
