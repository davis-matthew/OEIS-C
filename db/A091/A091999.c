/*
 * A091999  Numbers that are congruent to {2, 10} mod 12.
 * https://oeis.org/A091999
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*n - 3 + (-1)^n (equivalently: a(n) = 6*n - 4 for odd n,
 *            6*n - 2 for even n)
 * Range:     u8 n <= 43, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;          /* cast index to the sequence type */
    if (n & 1)                    /* odd n */
        return nn * 6 - 4;
    else                          /* even n */
        return nn * 6 - 2;
}
