/*
 * A080463  Sum of the two numbers formed by alternate digits of n.
 * https://oeis.org/A080463
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn, look
 * Formula:   a(n) = sum of the two numbers formed by alternating digits of n
 *            * taken from the leftmost position (1st, 3rd, 5th, ... vs 2nd,
 *            4th, 6th, ...)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Special case for n = 0 */
    if (n == 0)
        return 0;

    /* Extract decimal digits into a temporary array in reverse order */
    idx_t digits[20];       /* 20 digits are enough for a 64‑bit integer */
    idx_t count = 0;
    idx_t temp = n;
    while (temp > 0)
    {
        digits[count++] = temp % 10;
        temp /= 10;
    }

    /* Reconstruct the two numbers from most‑significant digit to least‑significant */
    seq_t odd  = 0;   /* digits at positions 1,3,5,... (index 0,2,4,...) */
    seq_t even = 0;   /* digits at positions 2,4,6,... (index 1,3,5,...) */
    for (idx_t i = 0; i < count; ++i)
    {
        idx_t d = digits[count - 1 - i];   /* most‑significant digit first */
        if ((i & 1) == 0)                  /* i is even → odd position */
            odd = odd * 10 + d;
        else                               /* i is odd  → even position */
            even = even * 10 + d;
    }
    return odd + even;
}
