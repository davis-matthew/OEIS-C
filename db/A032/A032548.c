/*
 * A032548  Integer part of decimal 'base-7 looking' numbers divided by their
 *          actual base-7 values.
 * https://oeis.org/A032548
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = floor(A007093(n)/n) where A007093(n) is the decimal
 *            number obtained by interpreting the base-7 representation of n
 *            as decimal digits
 * Range:     u8 n <= 866, u16 n <= 34999, u32 n <= 35000, u64 n <= 35000,
 *            u128 n <= 35000, big any n
 * Validated: standard, pass, 115 terms through n = 9639 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t dec_from_base7(idx_t n)
{
    seq_t m = 0;
    unsigned int digits[64];
    int sz = 0;

    if (n == 0) {
        return 0;
    }

    while (n > 0) {
        digits[sz++] = n % 7;
        n /= 7;
    }

    for (int i = sz - 1; i >= 0; --i) {
        m = m * 10 + digits[i];
    }

    return m;
}

seq_t a(idx_t n)
{
    seq_t m = dec_from_base7(n);
    return m / (seq_t)n;
}
