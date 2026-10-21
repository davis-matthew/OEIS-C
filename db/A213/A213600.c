/*
 * A213600  Triangle T(n,k) read by rows: Number of Dyck n-paths with midpoint
 *          at height k.
 * https://oeis.org/A213600
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = A053121(r,k)^2 where r,k satisfy * r(r+1)/2 <= n <
 *            (r+1)(r+2)/2 (row index), * k = n - r(r+1)/2 (column index)
 * Range:     u8 n <= 37, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* multiplicative binomial coefficient: binom(a,b) */
static seq_t binom(idx_t a, idx_t b) {
    if (b > a) return 0;
    if (b > a - b) b = a - b;             /* use symmetry */
    seq_t res = 1;
    for (idx_t i = 1; i <= b; ++i) {
        res = res * (a - b + i) / i;
    }
    return res;
}

/* A053121(n,k) – ballot numbers */
static seq_t ballot(idx_t n, idx_t k) {
    if (k > n) return 0;
    if (((n - k) & 1) != 0)            /* parity mismatch → 0 */
        return 0;
    idx_t half = (n - k) / 2;           /* (n-k)/2   */
    seq_t a = binom(n, half);
    seq_t b = (half > 0) ? binom(n, half - 1) : 0;
    return a - b;
}

seq_t a(idx_t n) {
    /* find row r such that r(r+1)/2 <= n < (r+1)(r+2)/2 */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) {
        ++r;
    }

    idx_t k = n - r * (r + 1) / 2;   /* column within the row */

    seq_t val = ballot(r, k);       /* A053121(r,k) */
    return val * val;               /* square to get T(r,k) */
}
