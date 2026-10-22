/*
 * A000529  Powers of rooted tree enumerator.
 * https://oeis.org/A000529
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 20 + 54*C(n-1,1) + 58*C(n-1,2) + 32*C(n-1,3) + 9*C(n-1,4)
 *            + C(n-1,5) (quintic polynomial in n; its 5th differences are
 *            constant 1)
 * Range:     u8 n <= 3, u16 n <= 18, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const unsigned coef[6] = {20, 54, 58, 32, 9, 1};
    idx_t m = n - 1;
    seq_t sum = 0;
    seq_t c = 1; /* C(m, k) */
    for (idx_t k = 0; k <= 5; k++) {
        if (k > 0) {
            if (k > m) break;
            c = c * ((seq_t)m - (seq_t)(k - 1)) / (seq_t)k;
        }
        sum += c * (seq_t)coef[k];
    }
    return sum;
}
