/*
 * A095617  a(n) = 113 written in base 10 - n.
 * https://oeis.org/A095617
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = representation of the decimal number 113 in base (10-n),
 *            interpreted as a decimal integer. For base 1 the result is the
 *            unary number consisting of 113 consecutive 1 digits. For base 0
 *            or |base|=1 (illegal bases) the result is defined to be 0
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   decimal_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Assumptions:
 * - seq_t and idx_t are predefined integer types by the build.
 * - The function must be linkable in C and C++ (C++ will use the same
 *   seq_t type, possibly an arbitrary‑precision integer class).
 *   All arithmetic uses +, -, *, /, % only.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/decimal_digits.h"

seq_t a(idx_t n)
{
    long long base = (long long)10 - (long long)n;

    /* Unary base 1 */
    if (base == 1) {
        seq_t res = 0;
        for (int i = 0; i < 113; ++i)
            res = res * (seq_t)10 + 1;
        return res;
    }

    /* Undefined or illegal bases */
    if (base == 0 || base == -1 || base == 1)
        return 0;

    long long abs_base = base >= 0 ? base : -base;
    long long value = 113;
    long long digits[32];
    int len = 0;

    if (value == 0) {
        digits[0] = 0;
        len = 1;
    } else {
        while (value != 0) {
            long long r = value % base;
            if (r < 0)
                r += abs_base;
            digits[len++] = r;
            value = (value - r) / base;
            /* Limit to avoid overflow of the array – 113 has at most 7
               digits in any integer base ≥ 2.  The array is over‑sized. */
        }
    }

    /* Build the decimal number by concatenating the digits */
    seq_t res = 0;
    for (int i = len - 1; i >= 0; --i) {
        long long d = digits[i];
        int mult = decimal_digits(d);
        for (int k = 0; k < mult; ++k)
            res = res * (seq_t)10;
        res += (seq_t)d;
    }

    return res;
}
