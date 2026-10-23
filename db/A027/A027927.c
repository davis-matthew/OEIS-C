/*
 * A027927  Number of plane regions after drawing (in general position) a
 *          convex n-gon and all its diagonals.
 * https://oeis.org/A027927
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + binomial(n,4) + binomial(n-1,2)
 * Range:     u8 n <= 10, u16 n <= 36, u32 n <= 567, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 2) return 0;                 /* offset is 2; value undefined below */
    seq_t result = 1;
    if (n >= 4) {
        seq_t N   = (seq_t)n;
        result += N * (N - 1) * (N - 2) * (N - 3) / 24;
    }
    if (n >= 3) {
        seq_t N1  = (seq_t)n - 1;
        result += N1 * (N1 - 1) / 2;
    }
    return result;
}
