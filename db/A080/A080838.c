/*
 * A080838  Orchard crossing number of complete bipartite graph K_{1,n}.
 * https://oeis.org/A080838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n/16)*(2*n^2 - 8*n + 7 + (-1)^n)
 * Range:     u8 n <= 14, u16 n <= 81, u32 n <= 1001, u64 n <= 1001,
 *            u128 n <= 1001, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Use signed 64-bit intermediates to handle negative terms safely */
    long long nn = (long long)n;
    long long term = 2 * nn * nn - 8 * nn + 7 + ((n & 1) ? -1 : 1);
    long long num  = nn * term;
    long long res  = num / 16;
    return (seq_t)res;
}
