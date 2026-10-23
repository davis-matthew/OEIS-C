/*
 * A187163  Number of 2-step self-avoiding walks on an n X n X n cube summed
 *          over all starting positions.
 * https://oeis.org/A187163
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*n^3 - 6*n^2
 * Range:     u8 n <= 3, u16 n <= 22, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast the index once to seq_t */
    seq_t nval = (seq_t)n;
    seq_t n2   = nval * nval;      /* n^2  */
    seq_t n3   = n2   * nval;      /* n^3  */
    seq_t six  = 6;
    return six * n3 - six * n2;
}
