/*
 * A203465  a(n) = A203305(n)/A000178(n) where A000178 are superfactorials.
 * https://oeis.org/A203465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=2..n} ( Product_{j=1..k-1} (2^k - 2^j) ) /
 *            (k-1)! [= A203305(n)/A000178(n-1)], a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Validated: standard, pass, 22 terms through n = 22 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t A203465_gcd(seq_t x, seq_t y) {
    while (y != 0) {
        seq_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

seq_t a(idx_t n)
{
    seq_t r = 1;
    if (n < 2) return r;
    for (idx_t k = 2; k <= n; ++k) {
        /* denominators 2..k-1 whose product is (k-1)! */
        size_t cnt = (size_t)k;
        seq_t *den = mem_new(seq_t, cnt + 1);
        for (idx_t i = 2; i < k; ++i) den[(size_t)i] = (seq_t)i;
        seq_t pk = (seq_t)1 << (unsigned)k;
        for (idx_t j = 1; j < k; ++j) {
            seq_t f = pk - ((seq_t)1 << (unsigned)j);
            for (idx_t i = 2; i < k; ++i) {
                seq_t d = den[(size_t)i];
                if (d > 1) {
                    seq_t g = A203465_gcd(f, d);
                    if (g > 1) {
                        f /= g;
                        den[(size_t)i] = d / g;
                    }
                }
            }
            r *= f;
        }
        mem_free(den);
    }
    return r;
}
