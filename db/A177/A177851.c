/*
 * A177851  Triangle read by rows: T(n, m) = binomial(n + m - 3, m - 1)*(2 * m
 *          + n - 2) / m, for n>=1 and 1<=m<=n.
 * https://oeis.org/A177851
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 54, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A177851:
   T(n, m) = binomial(n + m - 3, m - 1) * (2*m + n - 2) / m,
   for n ≥ 1 and 1 ≤ m ≤ n.
   a(n) is the n‑th element of the triangular array read row‑by‑row. */
#include <stdint.h>

/* helper: integer square‑root, floor of sqrt(x) for uint64_t */
static idx_t floor_isqrt_u64(idx_t x) {
    idx_t r = 0;
    while (r * r <= x) r++;
    return r - 1;
}

/* main function */
seq_t a(idx_t n) {
    if (n == 0) return 0;                     /* undefined, return 0 */

    /* Find r such that r*(r+1)/2 ≥ n  (row index) */
    idx_t lo = 0, hi = n;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if (mid * (mid + 1) / 2 >= n) hi = mid - 1;
        else lo = mid;
    }
    idx_t r = lo + 1;                         /* smallest r with T(r) >= n */

    /* Column index within the row */
    idx_t prev = (r - 1) * r / 2;
    idx_t m = n - prev;                       /* 1 ≤ m ≤ r */

    /* Compute binomial(r + m - 3, m - 1) multiplicatively */
    idx_t N = r + m - 3;
    idx_t K = m - 1;
    seq_t bin = 1;
    for (idx_t i = 1; i <= K; ++i) {
        bin = bin * (seq_t)(N - K + i) / (seq_t)i;
    }

    /* Final value */
    seq_t mult = (seq_t)(2 * m + r - 2);
    return bin * mult / (seq_t)m;
}
