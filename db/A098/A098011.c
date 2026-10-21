/*
 * A098011  10^a(n) + 1 = A088773(n).
 * https://oeis.org/A098011
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = a(2) = 1, a(3) = 2, a(n) = 3 * 2^(n‑4) for n > 3
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 34, u64 n <= 66, u128 n <= 130,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <limits.h>

seq_t a(idx_t n)
{
    if (n < 1)         /* out of range – return 0 as a safe default */
        return (seq_t)0;

    if (n == 1 || n == 2)
        return (seq_t)1;
    if (n == 3)
        return (seq_t)2;

    idx_t k = n - 4;   /* exponent of 2 */
    seq_t res = (seq_t)3;

    /* Shift left k times; safe for unsigned types (wraps modulo width). */
    while (k--)
        res <<= 1;

    return res;
}
