/*
 * A078818  a(n) = 30*binomial(2n,n)/(n+3).
 * https://oeis.org/A078818
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 30*binomial(2*n, n)/(n+3) Recurrence: a(0) = 10, a(n) =
 *            a(n-1)*(4*n^2 + 6*n - 4)/(n^2 + 3*n)
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 17, u64 n <= 33, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t gcd_seq(seq_t a, seq_t b) {
    while (b) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)10;

    seq_t res = (seq_t)10;          /* a(0) */

    for (idx_t k = 1; k <= n; ++k) {
        seq_t kseq = (seq_t)k;
        seq_t k2   = kseq * kseq;          /* k^2 */

        /* numerator = 4*k^2 + 6*k - 4 */
        seq_t num = (seq_t)4 * k2 + (seq_t)6 * kseq - (seq_t)4;

        /* denominator = k^2 + 3*k */
        seq_t den = k2 + (seq_t)3 * kseq;

        /* cancel common factors with current result */
        seq_t g1 = gcd_seq(res, den);
        res /= g1;
        den /= g1;

        /* cancel remaining common factors with numerator */
        seq_t g2 = gcd_seq(num, den);
        num /= g2;
        den /= g2;          /* den should now be 1 (exact division) */

        /* multiply and divide – exact because of the cancellations */
        res = (res * num) / den;
    }

    return res;
}
