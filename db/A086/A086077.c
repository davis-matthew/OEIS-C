/*
 * A086077  Number of 6's in decimal expansion of triangular number n(n+1)/2.
 * https://oeis.org/A086077
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = number of decimal digits '6' in T(n), where T(n) =
 *            n*(n+1)/2 is the n-th triangular number. For each n ≥ 0, compute
 *            T(n) as seq_t, then count the 6's
 * Range:     u8 n <= 22, u16 n <= 399, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Compute the n-th triangular number using seq_t arithmetic. */
    seq_t tri = (seq_t)n * ((seq_t)n + 1) / 2;
    /* Count digits equal to 6 in the decimal representation of tri. */
    seq_t cnt = 0;
    while (tri > 0) {
        if (tri % 10 == 6) cnt++;
        tri /= 10;
    }
    return cnt;
}
