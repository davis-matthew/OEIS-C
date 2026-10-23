/*
 * A014910  Inverse of 901st cyclotomic polynomial.
 * https://oeis.org/A014910
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = [x^n] (1 - x^17)(1 - x^53)/((1 - x)(1 - x^901))
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*         = floor(n/901)+1
 *           - (n>=17  ? floor((n-17)/901)+1 : 0)
 *           - (n>=53  ? floor((n-53)/901)+1 : 0)
 *           + (n>=70  ? floor((n-70)/901)+1 : 0)
 */

seq_t a(idx_t n) {
    const idx_t P = 901;           /* period length */
    seq_t result = 0;

    /* term with k = 0 (always present) */
    result += (seq_t)((n / P) + 1);

    /* term with k = 17 */
    if (n >= 17) {
        result -= (seq_t)(((n - 17) / P) + 1);
    }

    /* term with k = 53 */
    if (n >= 53) {
        result -= (seq_t)(((n - 53) / P) + 1);
    }

    /* term with k = 70 */
    if (n >= 70) {
        result += (seq_t)(((n - 70) / P) + 1);
    }

    return result;
}
