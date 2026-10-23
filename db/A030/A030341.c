/*
 * A030341  Triangle T(n,k): write n in base 3, reverse order of digits.
 * https://oeis.org/A030341
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, tabf, less
 * Formula:   not stated
 * Range:     u8 n <= 5914, u16 n <= 5914, u32 n <= 5914, u64 n <= 5914,
 *            u128 n <= 5914, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence a(n) is the concatenation of the base‑3 digits of successive
 * non‑negative integers, written least‑significant digit first.  Equivalently,
 * a(n) is the digit at position n when the base‑3 representations of
 * 0, 1, 2, … are written in little‑endian order and flattened. */
#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t rem = n;
    idx_t m = 0;
    for (;;)
    {
        /* digits in the base‑3 representation of m */
        idx_t digits;
        if (m == 0)
            digits = 1;
        else
        {
            digits = 0;
            idx_t t = m;
            while (t)
            {
                t /= 3;
                ++digits;
            }
        }
        if (rem < digits)
            break;
        rem -= digits;
        ++m;
    }

    /* compute 3^rem */
    idx_t pow3 = 1;
    for (idx_t i = 0; i < rem; ++i)
        pow3 *= 3;

    seq_t digit = (seq_t)((m / pow3) % 3);
    return digit;
}
