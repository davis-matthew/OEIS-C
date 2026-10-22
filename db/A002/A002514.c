/*
 * A002514  Coefficients in the asymptotic expansions of modified Hankel
 *          functions h_1(z) and h_2(z), rounded to nearest integer.
 * https://oeis.org/A002514
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round((Product_{k=1..n} (9*(2*k-1)^2 - 4)) / (2^(4*n) *
 *            3^n * n!)), using 9*(2k-1)^2 - 4 = (6k-5)*(6k-1)
 * Range:     u8 n <= 8, u16 n <= 11, u32 n <= 16, u64 n <= 24, u128 n <= 38,
 *            big any n
 * Helpers:   a002514_div, a002514_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/a002514_div.h"
#include "../utils/a002514_mul.h"

seq_t a(idx_t n) {
    size_t cap = 2 * (size_t)n + 8;
    uint32_t *x = (uint32_t *)calloc(cap, sizeof(uint32_t));
    size_t len = 1;
    idx_t k;
    x[0] = 2; /* compute 2*P / D, then round */

    for (k = 1; k <= n; k++) {
        uint64_t f1 = 6 * (uint64_t)k - 5, f2 = 6 * (uint64_t)k - 1;
        uint64_t m = f1 * f2;
        if ((m >> 32) == 0) {
            len = a002514_mul(x, len, (uint32_t)m);
        } else {
            len = a002514_mul(x, len, (uint32_t)f1);
            len = a002514_mul(x, len, (uint32_t)f2);
        }
    }

    /* divide by 3^n * n! = Product (3k) */
    k = 1;
    while (k <= n) {
        uint64_t d1 = 3 * (uint64_t)k;
        if (k + 1 <= n) {
            uint64_t d2 = 3 * (uint64_t)(k + 1);
            uint64_t d = d1 * d2;
            if ((d >> 32) == 0) {
                len = a002514_div(x, len, (uint32_t)d);
                k += 2;
                continue;
            }
        }
        len = a002514_div(x, len, (uint32_t)d1);
        k++;
    }

    /* shift right by 4n bits (floor of 2x) */
    {
        uint64_t s = 4 * (uint64_t)n;
        size_t w = (size_t)(s / 32);
        unsigned b = (unsigned)(s % 32);
        size_t i, nl;
        if (w >= len) {
            x[0] = 0;
            len = 1;
        } else {
            nl = len - w;
            for (i = 0; i < nl; i++) {
                uint64_t lo = x[i + w];
                uint64_t hi = (i + w + 1 < len) ? x[i + w + 1] : 0;
                uint64_t v = (b == 0) ? lo : ((lo >> b) | (hi << (32 - b)));
                x[i] = (uint32_t)v;
            }
            len = nl;
            while (len > 1 && x[len - 1] == 0) len--;
        }
    }

    /* round: (f + 1) >> 1 */
    {
        size_t i = 0;
        while (i < len) {
            x[i]++;
            if (x[i] != 0) break;
            i++;
        }
        if (i == len) x[len++] = 1;
        for (i = 0; i < len; i++) {
            uint32_t hi = (i + 1 < len) ? x[i + 1] : 0;
            x[i] = (x[i] >> 1) | (hi << 31);
        }
        while (len > 1 && x[len - 1] == 0) len--;
    }

    {
        seq_t r = 0;
        size_t i;
        int j;
        for (i = len; i-- > 0;) {
            for (j = 3; j >= 0; j--) {
                r <<= 8;
                r += (seq_t)((x[i] >> (8 * j)) & 0xFFu);
            }
        }
        free(x);
        return r;
    }
}
