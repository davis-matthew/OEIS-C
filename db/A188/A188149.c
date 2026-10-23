/*
 * A188149  Number of 4-step self-avoiding walks on an n X n square summed
 *          over all starting positions.
 * https://oeis.org/A188149
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 36*n^2 - 100*n + 56 for n>2 (a(1)=0, a(2)=8)
 * Range:     u8 n <= 4, u16 n <= 44, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 2) {
        if (n == 1) return (seq_t)0;
        if (n == 2) return (seq_t)8;
        /* n == 0 or negative (should not happen), return 0 */
        return (seq_t)0;
    }
    seq_t nn = (seq_t)n;
    return (seq_t)36 * nn * nn - (seq_t)100 * nn + (seq_t)56;
}
