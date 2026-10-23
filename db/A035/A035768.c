/*
 * A035768  Coordination sequence for C_31 lattice.
 * https://oeis.org/A035768
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = [x^(2n)] ((1+x)/(1-x))^31
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 11, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 0;
    seq_t bin1 = 1;                 // C(31,0)
    for (idx_t j = 0; j <= 31; ++j) {
        // skip terms where 2*n < j
        if (n < (j + 1) / 2) break;

        seq_t k = ((seq_t)n * 2) - j;   // k = 2n - j

        /* bin2 = binom(k+30, k) = binom(k+30, 30) */
        seq_t bin2 = 1;
        for (idx_t t = 1; t <= 30; ++t) {
            bin2 = bin2 * (k + t) / t;
        }

        result += bin1 * bin2;

        /* update bin1 to C(31, j+1) for next iteration */
        if (j < 31) {
            bin1 = bin1 * ((seq_t)(31 - j)) / ((seq_t)(j + 1));
        }
    }
    return result;
}
