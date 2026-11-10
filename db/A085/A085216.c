/*
 * A085216  Transpose of A085215.
 * https://oeis.org/A085216
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = A(x,y), the number whose factorial-base expansion
 *            (A007623) is that of x followed by that of y, where n = s(s+1)/2
 *            + x, y = s - x and 0 <= x <= s (transpose of A085215)
 * Range:     u8 n <= 70, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Validated: partial, pass, 73 terms through n = 72 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static seq_t A085216_a_impl(idx_t n)
{
    idx_t s = 0;
    while (((s + 1) * (s + 2)) / 2 <= n) ++s;
    idx_t x = n - (s * (s + 1)) / 2;
    idx_t y = s - x;

    /* y occupies the low factorial places; count its digits */
    unsigned len_y = 0;
    idx_t t = y;
    unsigned r = 2;
    while (t > 0) {
        t /= r;
        ++r;
        ++len_y;
    }

    seq_t result = (seq_t)y;

    /* weight of place len_y is (len_y+1)! */
    seq_t fact = 1;
    for (unsigned i = 2; i <= len_y + 1; ++i) fact *= (seq_t)i;

    t = x;
    unsigned k = 0;
    while (t > 0) {
        idx_t radix = (idx_t)(k + 2);
        idx_t d = t % radix;
        t /= radix;
        if (d) result += fact * (seq_t)d;
        fact *= (seq_t)(len_y + k + 2);
        ++k;
    }
    return result;
}

seq_t a(idx_t n)
{
    return A085216_a_impl(n);
}
