/*
 * A095551  a(n) = 79 written in base 15 - n.
 * https://oeis.org/A095551
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = 79 written in base (15 - n)
 * Range:     u8 n <= 9, u16 n <= 12, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/seq_from_str.h"

seq_t a(idx_t n)
{
    /* Base must be >= 1 for a legitimate representation.
       For n > 14 the base would be <= 0, so we return 0. */
    if (n > 14)
        return 0;

    int base = 15 - (int)n;
    if (base <= 1) {          /* base == 1 : unary representation */
        static char unary[80];
        int i;
        /* Fill with 79 '1' digits */
        for (i = 0; i < 79; ++i)
            unary[i] = '1';
        unary[79] = '\0';
        return seq_from_str(unary);
    }

    /* Convert 79 to the required base, store digits (least‑to‑most) */
    int digits[64];
    int len = 0;
    int temp = 79;
    while (temp > 0) {
        digits[len++] = temp % base;
        temp /= base;
    }

    /* Build decimal value from the digit sequence */
    seq_t val = 0;
    for (int i = len - 1; i >= 0; --i)
        val = val * 10 + digits[i];

    return val;
}
