/*
 * A007464  Shifts left under GCD-convolution with itself.
 * https://oeis.org/A007464
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, eigen
 * Formula:   a(0) = 1; a(n+1) = Sum_{i=0..n} gcd(a(i), a(n-i))
 * Range:     u8 n <= 44, u16 n <= 1958, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bgcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/bgcd.h"

seq_t a(idx_t n) {
    static uint64_t *c = NULL;
    static idx_t len = 0; /* number of computed terms */
    static idx_t cap = 0;

    if (n >= cap) {
        idx_t nc = (n + 1) * 2;
        if (nc < 256) nc = 256;
        uint64_t *p = (uint64_t *)realloc(c, (size_t)nc * sizeof(uint64_t));
        if (!p) return (seq_t)0;
        c = p;
        cap = nc;
    }
    if (len == 0) {
        c[0] = 1;
        len = 1;
    }
    while (len <= n) {
        idx_t k = len;          /* compute c[k] = sum_{i=0..k-1} gcd(c[i], c[k-1-i]) */
        idx_t m = k - 1;
        uint64_t s = 0;
        idx_t i;
        for (i = 0; i < m - i; i++)
            s += bgcd(c[i], c[m - i]);
        s *= 2;
        if (i == m - i) s += c[i];
        c[k] = s;
        len++;
    }
    return (seq_t)c[n];
}
