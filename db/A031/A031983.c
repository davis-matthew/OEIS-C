/*
 * A031983  Numbers with exactly five distinct base-6 digits.
 * https://oeis.org/A031983
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: iterate integers, keep those whose base‑6 representation contains exactly five distinct digits; return the n‑th such integer (offset 1). */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                       /* offset 1, value for n=0 undefined but safe */
    idx_t count = 0;
    for (idx_t x = 0; ; ++x)
    {
        seq_t cur = (seq_t)x;
        uint8_t mask = 0;
        if (cur == 0)
            mask = 1u;                   /* digit 0 only */
        else
        {
            while (cur > 0)
            {
                uint8_t digit = (uint8_t)(cur % 6);
                mask |= 1u << digit;
                cur /= 6;
            }
        }
        /* count how many distinct digits are present */
        int bits = 0;
        uint8_t m = mask;
        while (m)
        {
            bits += m & 1;
            m >>= 1;
        }
        if (bits == 5)
        {
            ++count;
            if (count == n)
                return (seq_t)x;
        }
    }
}
