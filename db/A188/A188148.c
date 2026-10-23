/*
 * A188148  Number of 3-step self-avoiding walks on an n X n square summed
 *          over all starting positions.
 * https://oeis.org/A188148
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*n*n - 24*n + 8 for n>1, a(1)=0
 * Range:     u8 n <= 5, u16 n <= 74, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n <= 1)
        return 0;
    seq_t nn = (seq_t)n;
    return 12 * nn * nn - 24 * nn + 8;
}
