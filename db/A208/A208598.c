/*
 * A208598  Number of 4-bead necklaces labeled with numbers -n..n not allowing
 *          reversal, with sum zero.
 * https://oeis.org/A208598
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (4*n^3 + 6*n^2 + 5*n + 3)/3 (for n ≥ 1)
 * Range:     u8 n <= 5, u16 n <= 36, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return 0;                  /* sequence is defined only for n ≥ 1 */

    seq_t nn = (seq_t)n;                  /* cast to seq_t first to avoid overflow of idx_t */
    seq_t n2 = nn * nn;                   /* n² */
    seq_t n3 = n2 * nn;                   /* n³ */

    /* numerator is guaranteed to be divisible by 3 for integer n */
    seq_t val = (4 * n3 + 6 * n2 + 5 * nn + 3) / 3;
    return val;
}
