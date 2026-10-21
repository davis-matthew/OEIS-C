/*
 * A095546  a(n) = 77 written in base n.
 * https://oeis.org/A095546
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = representation of decimal 77 in base n, concatenated as
 *            decimal digits
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 none, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Computes the OEIS sequence A095546: a(n) = 77 written in base n.
 * The result is given as the decimal integer formed by concatenating
 * the base‑n digits of 77 (each digit written in its decimal
 * representation).  For n = 1 the value consists of 77 ones. */
seq_t a(idx_t n)
{
    /* Special case n == 1: 77 ones */
    if (n == (idx_t)1)
    {
        seq_t r = 0;
        for (idx_t i = 0; i < 77; ++i)
            r = r * (seq_t)10 + (seq_t)1;
        return r;
    }

    /* Convert 77 to base n and store digits in reverse order */
    idx_t temp = (idx_t)77;
    int digits[10];
    int count = 0;
    while (temp > 0)
    {
        digits[count++] = (int)(temp % n);
        temp /= n;
    }

    /* Build the decimal concatenation */
    seq_t r = 0;
    for (int i = count - 1; i >= 0; --i)
    {
        idx_t d = (idx_t)digits[i];

        /* Determine number of decimal digits of d */
        int len = 1;
        if (d >= 10)       len = 2;
        if (d >= 100)      len = 3;
        if (d >= 1000)     len = 4;

        /* Compute 10^len */
        seq_t pow10 = 1;
        for (int j = 0; j < len; ++j)
            pow10 *= (seq_t)10;

        r = r * pow10 + (seq_t)d;
    }

    return r;
}
