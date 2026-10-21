/*
 * A213565  Principal diagonal of the convolution array A213564.
 * https://oeis.org/A213565
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(n+4,5) + 15*C(n+3,5) + 16*C(n+2,5) where C(k,5) =
 *            k*(k-1)*(k-2)*(k-3)*(k-4)/120
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 109, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t binom5(idx_t k) {
    /* k choose 5; returns 0 for k < 5 */
    if (k < 5)
        return (seq_t)0;
    seq_t res = 1;
    /* compute iteratively to avoid overflows: */
    for (idx_t i = 1; i <= 5; ++i) {
        /* multiply by the next factor, then divide by i */
        res = res * (k - 5 + i) / i;
    }
    return res;
}

seq_t a(idx_t n) {
    /* Compute a(n) using the binomial-formula above. */
    seq_t term1 = binom5(n + 4);          /* C(n+4,5)   */
    seq_t term2 = binom5(n + 3);          /* C(n+3,5)   */
    seq_t term3 = binom5(n + 2);          /* C(n+2,5)   */
    return term1 + (seq_t)15 * term2 + (seq_t)16 * term3;
}
