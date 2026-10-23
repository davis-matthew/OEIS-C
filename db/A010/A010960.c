/*
 * A010960  Binomial coefficient C(44,n).
 * https://oeis.org/A010960
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = binomial(44,n)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n > 44) {
        return (seq_t)0;
    }
    /* Use symmetry to reduce multiplications */
    idx_t k = n;
    if (k > 22) {
        k = 44 - k;
    }
    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i) {
        /* multiply first to avoid fractional intermediate */
        result = result * (seq_t)(44 - k + i) / (seq_t)i;
    }
    return result;
}
