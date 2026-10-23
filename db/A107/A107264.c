/*
 * A107264  Expansion of (1 - 3*x - sqrt((1-3*x)^2 - 4*3*x^2))/(2*3*x^2).
 * https://oeis.org/A107264
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(1)=3, for n≥2 a(n) = [3*(2*n+1)*a(n-1) +
 *            3*(n-1)*a(n-2)] / (n+2) (exact division)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    seq_t a0 = (seq_t)1;            /* a(0) */
    seq_t a1 = (seq_t)3;            /* a(1) */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t idx = (seq_t)i;
        /* numerator = 3*(2*i+1)*a1 + 3*(i-1)*a0 */
        seq_t term1 = a1 * ((seq_t)3 * ((seq_t)2 * idx + (seq_t)1));
        seq_t term2 = a0 * ((seq_t)3 * (idx - (seq_t)1));
        seq_t num = term1 + term2;
        /* division is guaranteed to be exact */
        a0 = a1;
        a1 = num / (idx + (seq_t)2);
    }
    return a1;
}
