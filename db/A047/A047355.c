/*
 * A047355  Numbers that are congruent to {0, 3} mod 7.
 * https://oeis.org/A047355
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (14*n - (-1)^n - 15)/4 Equivalent integer form (no
 *            negative intermediate values): a(n) = (14*n - 16)/4 if n is even
 *            a(n) = (14*n - 14)/4 if n is odd The implementation below
 *            computes this directly
 * Range:     u8 n <= 19, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val;
    if (n & 1) {                     /* odd n */
        val = (seq_t)14 * n - (seq_t)14;
    } else {                         /* even n */
        val = (seq_t)14 * n - (seq_t)16;
    }
    return val / (seq_t)4;
}
