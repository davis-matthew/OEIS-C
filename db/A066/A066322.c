/*
 * A066322  Variable terms in period-16 pattern of first differences of binary
 *          representation of base i-1 expansion of n.
 * https://oeis.org/A066322
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, sign, changed
 * Formula:   a(n) = A066321(16*n+4) - A066321(16*n+3), where A066321(m) is
 *            the base-(i-1) digit string of m read as a binary number
 * Range:     i8 none, i16 n <= 2, i32 n <= 818, i64 n <= 65535,
 *            i128 n <= 65535, big any n
 * Validated: standard, pass, 64 terms through n = 9924 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static seq_t A066321_a(int64_t n)
{
    int64_t a_real = n;
    int64_t b_imag = 0;
    seq_t result = 0;
    seq_t p = 1;

    while (a_real != 0 || b_imag != 0) {
        int r = (int)((a_real - b_imag) & 1LL);
        if (r) {
            result += p;
        }
        int64_t new_a = (-a_real + r + b_imag) >> 1;
        int64_t new_b = (-a_real + r - b_imag) >> 1;
        a_real = new_a;
        b_imag = new_b;
        p *= 2;
    }
    return result;
}

seq_t a(idx_t n)
{
    int64_t m = (int64_t)n * 16;
    return A066321_a(m + 4) - A066321_a(m + 3);
}
