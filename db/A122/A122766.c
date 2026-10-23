/*
 * A122766  Triangle read by rows: let p(n, x) = x*p(n-1, x) - p(n-2, x), then
 *          T(n, x) = d^2/dx^2 (p(n, x)).
 * https://oeis.org/A122766
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = 2*(-1)^C(r-k+1,2)*C(k+1,2)*C(floor((r+k+2)/2), k+1),
 *            where r is the smallest integer with r(r+1)/2 >= n and k = n -
 *            (r-1)r/2 (1‑based column)
 * Range:     i8 n <= 23, i16 n <= 125, i32 n <= 645, i64 n <= 1275,
 *            i128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* triangular number t(r) = r(r+1)/2 */
static inline idx_t triangular_idx(idx_t n) {
    return ((idx_t)n * (n + 1)) / 2;
}

/* binary search for the row r containing index n */
static idx_t find_row(idx_t n) {
    idx_t low = 1, high = 1;
    while (triangular_idx(high) < n) high <<= 1;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (triangular_idx(mid) >= n) high = mid;
        else low = mid + 1;
    }
    return low;
}

/* binomial coefficient binom(N,K) with 128‑bit intermediate */
static seq_t binom_idx(idx_t N, idx_t K) {
    if (K > N) return 0;
    if (K > N - K) K = N - K;
    __int128 res = 1;
    for (idx_t i = 1; i <= K; ++i) {
        res = res * (N - K + i);
        res = res / i;
    }
    return (seq_t)res;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;                     /* protect against offset 0 */
    idx_t row = find_row(n);
    idx_t prev_tri = triangular_idx(row - 1);
    idx_t k = n - prev_tri;                   /* 1‑based column index */

    /* C(k+1, 2) = k(k+1)/2 */
    seq_t c1 = ((seq_t)(k + 1) * (seq_t)k) / 2;

    /* a = floor((row + k + 2) / 2) */
    idx_t a_val = (row + k + 2) / 2;

    /* C(a_val, k+1) */
    seq_t c2 = binom_idx(a_val, k + 1);

    /* sign factor (-1)^C(row-k+1, 2) */
    idx_t m = row - k + 1;
    idx_t parity = (m * (m - 1)) / 2;
    int sign = (parity & 1) ? -1 : 1;

    /* final value */
    __int128 tmp = (__int128)c1 * (__int128)c2;
    tmp *= 2;
    if (sign < 0) tmp = -tmp;

    return (seq_t)tmp;
}
