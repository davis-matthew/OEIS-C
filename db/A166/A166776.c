/*
 * A166776  Number of nX2 1..3 arrays containing at least one of each value,
 *          all equal values connected, and rows considered as a single number
 *          in nondecreasing order.
 * https://oeis.org/A166776
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A166776
 * Formula: a(n) = (n^4 + 8*n^3 + 8*n^2 - 23*n + 6)/6   (n ≥ 1)
 * This implements the polynomial directly, using only integer
 * arithmetic so it works with seq_t of any unsigned width or an
 * arbitrary-precision integer class.
 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* not part of the sequence      */
    seq_t N   = (seq_t)n;
    seq_t n2  = N * N;                   /* n^2                           */
    seq_t n3  = n2 * N;                  /* n^3                           */
    seq_t n4  = n3 * N;                  /* n^4                           */
    seq_t temp = n4 + (seq_t)8 * n3
                 + (seq_t)8 * n2 + (seq_t)6;   /* n^4 + 8*n^3 + 8*n^2 + 6   */
    seq_t num  = temp - (seq_t)23 * N;   /* subtract 23*n (no underflow) */
    return num / (seq_t)6;               /* divide by 6                  */
}
