/*
 * A095439  a(n) = 22 written in base 11 - n.
 * https://oeis.org/A095439
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A095439
   Formula: a(n) = representation of the decimal number 22 in base (11 - n).
   For n > 10 the base is ≤ 0, so a(n) is undefined; the function returns 0.
   Complexity: O(1) per call, uses only integer arithmetic allowed by the
   specification. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Base = 11 - n; only defined for n <= 10. */
    if (n > 10u)
        return 0;

    unsigned base = 11u - (unsigned)n;

    /* Unary representation in base 1: 22 ones. */
    if (base == 1u) {
        seq_t val = 0;
        for (int i = 0; i < 22; ++i)
            val = val * 10 + 1;
        return val;
    }

    /* Represent the decimal number 22 in the given base. */
    unsigned temp = 22u;
    unsigned digits[6];   /* 22 in base 2 needs at most 5 digits. */
    int len = 0;

    while (temp > 0)
    {
        digits[len++] = temp % base;
        temp /= base;
    }

    seq_t val = 0;
    for (int i = len - 1; i >= 0; --i)
        val = val * 10 + digits[i];

    return val;
}
