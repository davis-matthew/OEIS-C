/*
 * A166521  a(n) = (6*n + 7*(-1)^n + 3)/2.
 * https://oeis.org/A166521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6*n + 7*(-1)^n + 3)/2 Equivalent to a(n) = 3*n + 5 when
 *            n is even, and a(n) = 3*n - 2 when n is odd
 * Range:     u8 n <= 83, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;
    if ((n & 1) == 0)   /* n is even */
        return tn * 3 + 5;
    else                /* n is odd */
        return tn * 3 - 2;
}
