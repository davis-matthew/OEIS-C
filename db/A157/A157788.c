/*
 * A157788  a(n) = 1482401250n^2 - 830253600n + 116250751.
 * https://oeis.org/A157788
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1482401250*n^2 - 830253600*n + 116250751
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)0;
    seq_t N = (seq_t)n;
    seq_t term1 = (seq_t)1482401250 * N * N;
    seq_t term2 = (seq_t)830253600 * N;
    seq_t result = term1 - term2 + (seq_t)116250751;
    return result;
}
