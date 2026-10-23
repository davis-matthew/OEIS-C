/*
 * A022146  Coordination sequence for root lattice B_4.
 * https://oeis.org/A022146
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 32*n^3 - 16*n^2 + 16*n, a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 512, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    seq_t tn = (seq_t)n;
    /* Compute 32*n^3 - 16*n^2 + 16*n using seq_t arithmetic. */
    seq_t tn2 = tn * tn;        /* n^2 */
    seq_t tn3 = tn2 * tn;       /* n^3 */
    return 32 * tn3 - 16 * tn2 + 16 * tn;
}
