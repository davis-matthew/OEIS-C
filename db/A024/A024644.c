/*
 * A024644  n written in fractional base 8/2.
 * https://oeis.org/A024644
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 1919, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = n represented in fractional base 8/2:
   repeatedly take the least significant digit d = n mod 8,
   then set n = (n - d) / 4; the digits are collected in reverse
   order and concatenated as decimal digits. */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* n = 0 → trivial output */
    if (n == 0)
        return 0;

    /* Use a seq_t variable so that the algorithm works uniformly
       for both fixed‑width integer builds and arbitrary‑precision
       builds. */
    seq_t temp = n;

    /* Buffer for at most 64 digits – enough for any 128‑bit number
       because 4^32 > 2^128. */
    seq_t digits[64];
    size_t cnt = 0;

    while (temp != 0)
    {
        seq_t d = temp % 8;          /* current digit (0 … 7) */
        digits[cnt++] = d;
        temp = (temp - d) / 4;       /* shift to the next higher place */
    }

    /* Assemble the result from most significant digit to least. */
    seq_t result = 0;
    for (size_t i = cnt; i > 0; --i)
        result = result * 10 + digits[i - 1];

    return result;
}
