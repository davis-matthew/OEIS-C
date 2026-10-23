/*
 * A048445  Take the first n numbers written in base 14, concatenate them,
 *          then convert from base 14 to base 10.
 * https://oeis.org/A048445
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   concatenate decimal numbers 1..n written in base 14, interpret
 *            the result as a base‑14 number, then convert to base 10
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 23,
 *            big any n
 * Helpers:   base14_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/base14_digits.h"

seq_t a(idx_t n)
{
    seq_t result = 0;
    for (idx_t i = 1; i <= n; ++i) {
        /* number of base‑14 digits of i */
        idx_t len = base14_digits(i);

        /* 14ⁿ where n = len */
        seq_t pow14 = 1;
        for (idx_t k = 0; k < len; ++k)
            pow14 *= 14;

        result = result * pow14 + i;
    }
    return result;
}
