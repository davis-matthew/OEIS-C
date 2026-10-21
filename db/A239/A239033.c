/*
 * A239033  Number of 6 X n 0..2 arrays with no element equal to the sum of
 *          elements to its left or one plus the sum of the elements above it,
 *          modulo 3.
 * https://oeis.org/A239033
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1}^{min(12,n)} coeff[k] * C(n-k+9, 9) where coeff =
 *            [0, 8, -37, 142, -339, 592, -811, 996, -1020, 792, -377, 88, 8]
 *            a(1)=8, a(2)=43, a(3)=212, a(4)=806, a(5)=2592, a(6)=7265,
 *            a(7)=18362, a(8)=42809, a(9)=93464, a(10)=193157, a(11)=380900,
 *            a(12)=721154
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 33, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* multiplicative binomial coefficient C(n,k) for small k (k <= 9) */
static seq_t binomial(idx_t n, idx_t k) {
    if (k > n) return 0;
    if (k > n - k) k = n - k;        /* use smaller k */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (n - k + i);
        res = res / i;                 /* division is exact */
    }
    return res;
}

seq_t a(idx_t n) {
    if (n == 0) return 0; /* outside the offset */

    /* coefficients for the numerator polynomial after shifting by x */
    static const int coeff[13] = {
        0,    /* dummy for index 0 */
        8,    /* k = 1 */
       -37,   /* k = 2 */
       142,   /* k = 3 */
      -339,   /* k = 4 */
       592,   /* k = 5 */
      -811,   /* k = 6 */
       996,   /* k = 7 */
     -1020,   /* k = 8 */
       792,   /* k = 9 */
      -377,   /* k = 10 */
        88,   /* k = 11 */
        8    /* k = 12 */
    };

    seq_t pos = (seq_t)0;
    seq_t neg = (seq_t)0;
    idx_t maxk = n < 12 ? n : 12;     /* only terms up to n contribute */

    for (idx_t k = 1; k <= maxk; ++k) {
        seq_t bin = binomial(n - k + 9, 9);
        int c = coeff[k];
        if (c >= 0) {
            pos += (seq_t)c * bin;
        } else {
            neg += (seq_t)(-c) * bin;
        }
    }
    return pos - neg;   /* pos ≥ neg; subtraction safe for unsigned types */
}
