/*
 * A196848  Coefficient array of numerator polynomials of the ordinary
 *          generating functions for the alternating sums of powers for the
 *          numbers 1,2,...,2*n+1.
 * https://oeis.org/A196848
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, tabf
 * Formula:   a(n) is the (row, column) coefficient a(r,m) = [x^m] Go(r,x) *
 *            Π_{j=1}^{2r+1} (1 – j·x), with Go(r,x) = Σ_{k≥0} (
 *            Σ_{j=1}^{2r+1} (–1)^{j+1} j^k ) x^k/k!
 * Range:     i8 n <= 10, i16 n <= 19, i32 n <= 44, i64 n <= 48, i128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* integer square root: floor(sqrt(n)) */
static idx_t isqrt(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        idx_t sq = (idx_t)((__uint128_t)mid * mid);
        if (sq <= n) {
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return hi;
}

/* compute the n‑th term of A196848 */
seq_t a(idx_t n)
{
    /* map linear index n to row r and column m */
    idx_t r = isqrt(n);
    /* due to rounding, adjust r when (r+1)^2 <= n */
    while ((r + 1) * (r + 1) <= n) r++;
    while (r * r > n) r--;           /* safety for edge cases           */
    idx_t m = n - r * r;              /* 0 <= m < 2*r+1                   */

    /* length of the current row and product polynomial   */
    idx_t len = 2 * r + 1;            /* number of columns (2r+1)         */

    /* e[k] = Σ_{j=1}^{len} (-1)^{j+1} j^k      (k = 0 … len-1)   */
    seq_t *e = (seq_t *)malloc(sizeof(seq_t) * len);
    if (!e) return (seq_t)0;          /* out of memory, return 0          */
    for (idx_t k = 0; k < len; ++k) e[k] = 0;

    for (idx_t j = 1; j <= len; ++j) {
        seq_t pow = 1;                /* j^k */
        seq_t sign = (j & 1) ? 1 : -1;/* (-1)^{j+1} */
        for (idx_t k = 0; k < len; ++k) {
            e[k] += sign * pow;
            pow *= (seq_t)j;
        }
    }

    /* d[t] = coefficients of D(t)=Π_{j=1}^{len}(1-j t)   (t = 0 … len)   */
    seq_t *d = (seq_t *)malloc(sizeof(seq_t) * (len + 1));
    if (!d) { free(e); return (seq_t)0; }
    for (idx_t i = 0; i <= len; ++i) d[i] = 0;
    d[0] = 1;                         /* initial polynomial 1            */
    idx_t cur_deg = 0;                 /* current degree of d            */

    for (idx_t j = 1; j <= len; ++j) {
        seq_t prev = d[cur_deg];        /* old highest coefficient        */
        /* update existing coefficients descending */
        for (idx_t k = cur_deg; k >= 1; --k) {
            d[k] = d[k] - (seq_t)j * d[k - 1];
        }
        /* set new highest degree coefficient */
        d[cur_deg + 1] = -(seq_t)j * prev;
        cur_deg++;
    }

    /* a(r,m) = Σ_{t=0}^{m} d[t] * e[m-t]                     */
    seq_t result = 0;
    for (idx_t t = 0; t <= m; ++t) {
        result += d[t] * e[m - t];
    }

    free(e);
    free(d);
    return result;
}
