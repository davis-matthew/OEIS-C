/*
 * A187165  Number of 4-step self-avoiding walks on an n X n X n cube summed
 *          over all starting positions.
 * https://oeis.org/A187165
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 150*n^3 - 426*n^2 + 312*n - 48 for n > 2 Special cases:
 *            a(1) = 0, a(2) = 96 Computed directly in seq_t to avoid overflow
 *            on large n
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == (idx_t)1) return (seq_t)0;
    if (n == (idx_t)2) return (seq_t)96;
    seq_t ns = (seq_t)n;
    seq_t a = (seq_t)150 * ns * ns * ns
            - (seq_t)426 * ns * ns
            + (seq_t)312 * ns
            - (seq_t)48;
    return a;
}
