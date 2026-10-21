/*
 * A086113  Number of 3 X n (0,1) matrices such that each row and each column
 *          is nondecreasing or nonincreasing.
 * https://oeis.org/A086113
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n*(n^2 + 3*n - 1)
 * Range:     u8 n <= 4, u16 n <= 31, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t val = nn * nn + (seq_t)3 * nn - (seq_t)1;
    return (seq_t)2 * nn * val;
}
