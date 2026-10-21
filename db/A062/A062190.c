/*
 * A062190  Coefficient triangle of certain polynomials N(5; m,x).
 * https://oeis.org/A062190
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the row‑wise list of the triangle T(n,k) = binom(n,k) *
 *            binom(n+5,k)
 * Range:     u8 n <= 12, u16 n <= 40, u32 n <= 160, u64 n <= 576,
 *            u128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

/* compute binomial coefficient 0 <= k <= n  */
static seq_t binom(idx_t n, idx_t k) {
    if (k > n) return 0;
    if (k == 0 || k == n) return 1;
    if (k > n - k) k = n - k;        /* use symmetry for smaller k */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i) / (seq_t)i;
    }
    return res;
}

/* determine row r and column c from the linear index n */
static void index_to_rc(idx_t n, idx_t *row, idx_t *col) {
    long double s = sqrt((long double)(8.0L * (long double)n + 1.0L));
    idx_t r = (idx_t)((s - 1.0L) / 2.0L);
    /* adjust in case of floating‑point rounding */
    while (((unsigned __int128)(r + 1) * (r + 2)) / 2 <= n) r++;
    while (((unsigned __int128)r * (r + 1)) / 2 > n) r--;
    *row = r;
    *col = n - (unsigned __int128)r * (r + 1) / 2;
}

seq_t a(idx_t n) {
    idx_t r, c;
    index_to_rc(n, &r, &c);
    seq_t t1 = binom(r, c);
    seq_t t2 = binom(r + 5, c);
    return t1 * t2;
}
