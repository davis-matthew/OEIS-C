/*
 * A232982  The Gauss factorial n_6!.
 * https://oeis.org/A232982
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{1 ≤ j ≤ n, gcd(j,6)=1} j
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 28, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t prod = (seq_t)1;
    for (idx_t j = 1; j <= n; ++j) {
        if ((j & 1) && (j % 3 != 0)) {
            prod *= (seq_t)j;
        }
    }
    return prod;
}
