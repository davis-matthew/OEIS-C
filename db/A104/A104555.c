/*
 * A104555  Expansion of x*(1 - x)/(1 - x + x^2)^3.
 * https://oeis.org/A104555
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 0 if n % 3 == 0; otherwise let k = (n + 2) / 3. a(3k-2) =
 *            (-1)^(k+1) * k*(3k-1)/2 a(3k-1) = (-1)^(k+1) * k*(3k+1)/2
 * Range:     i8 n <= 18, i16 n <= 318, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if ((n % 3) == 0) {
        return 0;
    }
    idx_t k = (n + 2) / 3;      /* k >= 1 */
    seq_t sign = (((k + 1) & 1) ? (seq_t)-1 : (seq_t)1);
    idx_t r = n % 3;            /* r == 1 or 2 */
    seq_t k_s = (seq_t)k;
    seq_t num = k_s * ((seq_t)(3 * k) + (r == 1 ? (seq_t)-1 : (seq_t)1));
    seq_t val = num / 2;        /* exact division by 2 */
    return sign * val;
}
