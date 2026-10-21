/*
 * A070596  n^5 mod 11.
 * https://oeis.org/A070596
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^5 mod 11
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Reduce n modulo 11 to avoid overflow, then compute the fifth power modulo 11 */
    idx_t r = n % 11;
    if (r == 0)
        return (seq_t)0;

    seq_t res = 1;
    seq_t base = (seq_t)r;
    for (int i = 0; i < 5; ++i)
        res = (res * base) % (seq_t)11;

    return res;
}
