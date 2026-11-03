/*
 * A001098  Multiply previous term by 2 and write in binary.
 * https://oeis.org/A001098
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1) = 1; a(n) = the decimal digits of the binary expansion of
 *            2*a(n-1), where a(n-1) is read as an ordinary decimal number
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Validated: standard, pass, 7 terms through n = 7 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t a001098_step(seq_t prev) {
    seq_t v = prev + prev;
    size_t cap = 64, len = 0;
    unsigned char *b = (unsigned char *)malloc(cap);
    /* extract bits of v, least significant first, 8 at a time */
    while (v > 0) {
        unsigned c = (unsigned)(long long)(v & 255);
        v >>= 8;
        if (len + 8 > cap) {
            cap *= 2;
            b = (unsigned char *)realloc(b, cap);
        }
        for (int k = 0; k < 8; k++) b[len++] = (unsigned char)((c >> k) & 1u);
    }
    while (len > 0 && b[len - 1] == 0) len--;

    static const unsigned p10[10] = {1u, 10u, 100u, 1000u, 10000u, 100000u,
                                     1000000u, 10000000u, 100000000u, 1000000000u};
    seq_t r = 0;
    size_t i = len;
    size_t g = len % 9;
    if (g == 0) g = 9;
    /* Horner in base 10^9: write the bits as decimal digits, most significant first */
    while (i > 0) {
        unsigned d = 0;
        for (size_t j = 0; j < g; j++) d = d * 10u + b[i - 1 - j];
        i -= g;
        r = r * (seq_t)p10[g] + (seq_t)d;
        g = 9;
    }
    free(b);
    return r;
}

seq_t a(idx_t n) {
    seq_t x = 1;
    for (idx_t i = 2; i <= n; i++) x = a001098_step(x);
    return x;
}
