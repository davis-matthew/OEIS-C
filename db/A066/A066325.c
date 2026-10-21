/*
 * A066325  Coefficients of unitary Hermite polynomials He_n(x).
 * https://oeis.org/A066325
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, tabl
 * Formula:   a(n) is the n‑th element of the triangle of coefficients of the
 *            unitary Hermite polynomials He_r(x). The triangle is listed row
 *            by row (row r has length r+1). For a given n, find the unique r
 *            with t_r = r(r+1)/2 <= n < t_{r+1} = (r+1)(r+2)/2 and let k = n
 *            - t_r (0 ≤ k ≤ r). Then if (r-k) is odd :...
 * Range:     i8 n <= 22, i16 n <= 56, i32 n <= 211, i64 n <= 563,
 *            i128 n <= 1544, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* ---------- find the row r by binary search on triangular numbers ---------- */
    idx_t low = 0, high = 1;
    /* Grow high until t_high > n */
    while ((high * (high + 1)) / 2 <= n) high <<= 1;
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        if ((mid * (mid + 1)) / 2 <= n)
            low = mid + 1;
        else
            high = mid;
    }
    idx_t r = low - 1;                     /* row index */
    idx_t row_start = (r * (r + 1)) / 2;    /* first index of this row */
    idx_t k = n - row_start;                /* column index in the row */

    /* ---------- compute the coefficient ---------- */
    if (((r - k) & 1) != 0)                 /* coefficient is zero if parity mismatched */
        return (seq_t)0;

    idx_t t = (r - k) >> 1;                 /* t = (r-k)/2 */

    /* binomial(r,k) via multiplicative formula */
    seq_t coeff = 1;
    idx_t kk = k;
    if (kk > r - kk) kk = r - kk;          /* use the smaller side for fewer products */
    for (idx_t i = 1; i <= kk; ++i) {
        coeff *= (seq_t)(r - i + 1);
        coeff /= (seq_t)i;
    }

    /* multiply by (t+1)*(t+2)*...*(2t)  (= (2t)!/t!) */
    for (idx_t i = 1; i <= t; ++i)
        coeff *= (seq_t)(t + i);

    /* divide by 2^t */
    seq_t pow2 = 1;
    for (idx_t i = 0; i < t; ++i)
        pow2 *= 2;
    coeff /= pow2;

    /* apply sign (-1)^t */
    if (t & 1)
        coeff = -coeff;

    return coeff;
}
