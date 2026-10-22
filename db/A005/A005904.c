/*
 * A005904  Centered dodecahedral numbers.
 * https://oeis.org/A005904
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n+1)*(5*n^2+5*n+1)
 * Range:     u8 n <= 2, u16 n <= 18, u32 n <= 753, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t s_n = (seq_t)n;
    seq_t term1 = 2 * s_n + 1;
    seq_t term2 = 5 * s_n * s_n + 5 * s_n + 1;
    return term1 * term2;
}
