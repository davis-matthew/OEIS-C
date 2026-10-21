/*
 * A081122  10th binomial transform of (1,1,0,0,0,0,...).
 * https://oeis.org/A081122
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n + 10) * 10^(n-1) (with a(0) = 1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;

    /* compute 10^(n-1) */
    seq_t p = 1;
    for (idx_t i = 1; i < n; ++i) {
        p *= (seq_t)10;
    }

    /* multiply by (n+10) */
    seq_t base = (seq_t)(n + 10);
    return base * p;
}
