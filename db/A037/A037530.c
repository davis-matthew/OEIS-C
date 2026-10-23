/*
 * A037530  Base-5 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,1,2.
 * https://oeis.org/A037530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = a(n-1)*5 + pattern[(n-1)%3], a(1) = 1, pattern = {1,1,2}
 *            (digits of the base‑5 representation)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* If the index is 0 or negative (not expected), return 0. */
    if (n == 0) return (seq_t)0;

    static const int pattern[3] = {1, 1, 2};
    seq_t result = 0;
    for (idx_t i = 0; i < n; ++i)
    {
        /* result = result * 5 + pattern[i % 3] */
        result = (result * (seq_t)5) + (seq_t)pattern[i % 3];
    }
    return result;
}
