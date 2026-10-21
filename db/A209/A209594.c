/*
 * A209594  Number of 3 X 3 0..n arrays with every element equal to a diagonal
 *          or antidiagonal reflection.
 * https://oeis.org/A209594
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1)^6 * (2*n+1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 20, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Offset is 1: a(1) is first term. For n==0 we return 0 for safety. */
    if (n == 0)
        return 0;
    seq_t m = (seq_t)n + 1;
    seq_t pow6 = m * m * m * m * m * m;
    seq_t res  = pow6 * ((m << 1) - 1);
    return res;
}
