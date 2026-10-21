/*
 * A063213  Dimension of the space of weight 2n cuspidal newforms for
 *          Gamma_0(45).
 * https://oeis.org/A063213
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n - 2*floor((n-1)/3) - 3
 * Range:     u8 n <= 50, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to seq_t for all arithmetic. */
    seq_t nn   = (seq_t)n;
    /* floor((n-1)/3) is simply integer division for positive integers. */
    seq_t div  = ((seq_t)(n - 1)) / 3;
    /* Compute the value using only integer operations. */
    return 4 * nn - 2 * div - 3;
}
