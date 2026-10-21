/*
 * A054131  T(2n,n), array T as in A054126.
 * https://oeis.org/A054131
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(n-1) + Sum_{m=0..n} binomial(3*n, m) (n ≥ 1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) is defined to be 2. */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)2;

    /* Compute S = sum_{m=0}^{n} C(3n, m) */
    seq_t sum = 1;          /* C(3n, 0) = 1 */
    seq_t binom = 1;        /* current binomial value */

    for (idx_t m = 1; m <= n; ++m) {
        seq_t num = (seq_t)(3 * n - m + 1);
        seq_t den = (seq_t)m;
        binom = binom * num / den;   /* exact division */
        sum += binom;
    }

    /* Compute 2^(n-1) */
    seq_t pow2 = (seq_t)1;
    for (idx_t i = 1; i < n; ++i) {
        pow2 <<= 1;   /* multiply by 2 */
    }

    return pow2 + sum;
}
