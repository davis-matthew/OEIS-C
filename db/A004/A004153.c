/*
 * A004153  Factorial numbers written backwards.
 * https://oeis.org/A004153
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A004086(A000142(n)), i.e. the decimal digits of n! read
 *            in reverse order
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 14, u64 n <= 23, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    /* n! in decimal limbs of base 1e9, least significant first */
    size_t cap = 16, len = 1;
    uint32_t *d = (uint32_t *)malloc(cap * sizeof(uint32_t));
    d[0] = 1;
    for (idx_t i = 2; i <= n; i++) {
        uint64_t carry = 0;
        for (size_t k = 0; k < len; k++) {
            uint64_t t = (uint64_t)d[k] * (uint64_t)i + carry;
            d[k] = (uint32_t)(t % 1000000000ULL);
            carry = t / 1000000000ULL;
        }
        while (carry) {
            if (len == cap) {
                cap *= 2;
                d = (uint32_t *)realloc(d, cap * sizeof(uint32_t));
            }
            d[len++] = (uint32_t)(carry % 1000000000ULL);
            carry /= 1000000000ULL;
        }
    }

    /* Reverse: least significant digits of n! become the most significant */
    seq_t r = 0;
    for (size_t k = 0; k < len; k++) {
        uint32_t v = d[k];
        uint32_t rev = 0, pw = 1;
        int digits;
        if (k + 1 < len) {
            digits = 9;
        } else {
            digits = 0;
            uint32_t t = v;
            while (t) { digits++; t /= 10; }
        }
        for (int j = 0; j < digits; j++) {
            rev = rev * 10 + v % 10;
            v /= 10;
            pw *= 10;
        }
        r = r * (seq_t)pw + (seq_t)rev;
    }
    free(d);
    return r;
}
