/*
 * A172088  Triangle: T(n,m) = n!! - m!! - (n-m)!! read by rows 0 <= m <= n,
 *          where ()!! are the double factorials.
 * https://oeis.org/A172088
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, easy
 * Formula:   a(n) is the nth element of the triangle T(r,m) = r!! - m!! -
 *            (r-m)!! read by rows. * For index n, let r be the unique integer
 *            with * r*(r+1)/2 <= n < (r+1)*(r+2)/2 * and let m = n -
 *            r*(r+1)/2. Then a(n) = r!! - m!! - (r-m)!!
 * Range:     i8 n <= 36, i16 n <= 78, i32 n <= 210, i64 n <= 595,
 *            i128 n <= 1653, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t double_factorial(idx_t n) {
    seq_t res = 1;
    for (idx_t k = n; k > 1; k -= 2) {
        res *= (seq_t)k;
    }
    return res;
}

extern seq_t a(idx_t n) {
    idx_t r = 0;
    /* find r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    while (((r + 1) * (r + 2)) / 2 <= n) {
        ++r;
    }
    idx_t offset = r * (r + 1) / 2;
    idx_t m = n - offset;

    seq_t nf = double_factorial(r);
    seq_t mf = double_factorial(m);
    seq_t rmf = double_factorial(r - m);

    return nf - mf - rmf;
}
