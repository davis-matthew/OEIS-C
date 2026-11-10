/*
 * A078277  a(n) = A078274(n)/11.
 * https://oeis.org/A078277
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A078274(n)/11, where A078274(n) interleaves the digits of
 *            the concatenation of the first n odd numbers (A019519) with its
 *            reverse (A038395)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 12,
 *            big any n
 * Validated: partial, pass, 12 terms through n = 12 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

static size_t A078277_digits(uint64_t m, uint8_t *out)
{
    uint8_t tmp[24];
    size_t l = 0;
    while (m > 0) {
        tmp[l++] = (uint8_t)(m % 10);
        m /= 10;
    }
    for (size_t i = 0; i < l; ++i)
        out[i] = tmp[l - 1 - i];
    return l;
}

seq_t a(idx_t n)
{
    uint64_t top = 2 * (uint64_t)n - 1;
    size_t total = 0;
    uint8_t buf[24];
    for (uint64_t m = 1; m <= top; m += 2)
        total += A078277_digits(m, buf);

    uint8_t *asc = (uint8_t *)malloc(total ? total : 1);
    uint8_t *rev = (uint8_t *)malloc(total ? total : 1);
    if (asc == NULL || rev == NULL) {
        free(asc);
        free(rev);
        return (seq_t)0;
    }

    size_t idx = 0;
    for (uint64_t m = 1; m <= top; m += 2)
        idx += A078277_digits(m, asc + idx);

    idx = 0;
    for (uint64_t m = top;; m -= 2) {
        idx += A078277_digits(m, rev + idx);
        if (m == 1) break;
    }

    /* long division by 11 while interleaving digits */
    seq_t q = (seq_t)0;
    unsigned rem = 0;
    for (size_t i = 0; i < total; ++i) {
        for (int k = 0; k < 2; ++k) {
            unsigned d = (k == 0) ? asc[i] : rev[i];
            unsigned cur = rem * 10 + d;
            q = q * 10 + (seq_t)(cur / 11);
            rem = cur % 11;
        }
    }

    free(asc);
    free(rev);
    return q;
}
