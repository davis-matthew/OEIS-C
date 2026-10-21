/*
 * A055096  Triangle read by rows, sums of 2 distinct nonzero squares: T(n,k)
 *          = k^2+n^2, (n>=2, 1 <= k <= n-1).
 * https://oeis.org/A055096
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = k^2 + r^2 * where r is the row (r >= 2) such that *
 *            (r-1)*r/2 <= n-2 < r*(r+1)/2 * and k = (n-2) - (r-1)*r/2 + 1 (1
 *            <= k <= r-1) * This gives the highest‑indexed term (triangle
 *            read by rows) * of the OEIS sequence A055096
 * Range:     u8 n <= 66, u16 n <= 7261, u32 n <= 7261, u64 n <= 7261,
 *            u128 n <= 7261, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* n >= 2 for the sequence; behaviour for n < 2 is unused. */
    idx_t idx = n - 2;                 /* zero‑based position in triangle */

    /* Find the largest t such that t*(t-1)/2 <= idx.
       t will be r-1. Use binary search with 128‑bit intermediate to avoid overflow. */
    idx_t low = 0;
    idx_t high = 4000000001ULL; /* upper bound for t (fits 64‑bit and safe for idx up to ~2^64) */
    while (low < high) {
        idx_t mid = (low + high + 1) / 2;
        __int128 val = (__int128)mid * (mid - 1) / 2;
        if (val <= idx) {
            low = mid;
        } else {
            high = mid - 1;
        }
    }
    idx_t t = low;                 /* t = r-1 */
    idx_t r = t + 1;               /* row number */

    /* Number of entries before row r: C = t*(t-1)/2 */
    idx_t C = t * (t - 1) / 2;
    idx_t k_index = idx - C;       /* zero‑based column in row r */
    idx_t k = k_index + 1;         /* 1‑based column (k ranges 1..r-1) */

    /* Compute k^2 + r^2 in seq_t */
    seq_t sq_k = (seq_t)k * k;
    seq_t sq_r = (seq_t)r * r;
    return sq_k + sq_r;
}
