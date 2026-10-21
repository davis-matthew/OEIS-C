/*
 * A220670  Coefficient triangle for powers of x^2 of polynomials appearing in
 *          a generalized Melham conjecture on alternating sums of third
 *          powers of Chebyshev's S polynomials with odd indices. Coefficients
 *          in powers of x^2 of 2 + (-1)^n*S(2*n,x).
 * https://oeis.org/A220670
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) is the flattened coefficient of the triangle a(r,p) = (p==0
 *            ? 2 : 0) + (-1)^p * binom(r+p, r-p), where r =
 *            floor((sqrt(8*n+1)-1)/2) is the row index, p = n - r(r+1)/2 is
 *            the column index
 * Range:     i8 n <= 30, i16 n <= 65, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* -------------------------------------- */
#include <stdint.h>

/* helper: find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
static inline idx_t row_from_flat(idx_t n) {
    if (n == 0) return 0;
    idx_t low = 0, high = 1;
    while (((__int128)high * (high + 1)) / 2 <= n) high <<= 1;
    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        if (((__int128)mid * (mid + 1)) / 2 <= n)
            low = mid;
        else
            high = mid;
    }
    return low;
}

/* helper: compute binomial coefficient binom(n,k) */
static seq_t binomial(idx_t n, idx_t k) {
    if (k > n - k) k = n - k;
    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i) {
        idx_t num = n - k + i;
        idx_t den = i;
        __int128 temp = (__int128)result * num;
        result = (seq_t)(temp / den);
    }
    return result;
}

/* ------------------------------------------------------------ */
seq_t a(idx_t n) {
    idx_t r = row_from_flat(n);
    idx_t p = n - (idx_t)(((__int128)r * (r + 1)) / 2);
    seq_t bin = binomial(r + p, r - p);
    seq_t val = (p == 0 ? (seq_t)2 : (seq_t)0);
    if (p & 1) val -= bin; else val += bin;
    return val;
}
