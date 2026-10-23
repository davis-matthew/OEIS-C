/*
 * A010932  Binomial coefficient C(16,n).
 * https://oeis.org/A010932
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = binomial(16, n) for 0 <= n <= 16, otherwise 0
 * Range:     u8 n <= 2, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Return 0 for indices outside the row 16 of Pascal's triangle. */
    if (n > 16) {
        return 0;
    }

    /* Use symmetry: C(16, n) = C(16, 16-n).  For n > 8 it suffices to
       compute with the smaller n to keep the loop short. */
    if (n > 8) {
        n = 16 - n;
    }

    seq_t result = 1;
    /* Compute the binomial coefficient multiplicatively. */
    for (idx_t i = 1; i <= n; ++i) {
        result = result * ((seq_t)(16 - i + 1)) / (seq_t)i;
    }
    return result;
}
