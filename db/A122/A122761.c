/*
 * A122761  Triangular read by rows: T(n, k) = 3^k * (1 + (n mod 2)).
 * https://oeis.org/A122761
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(row, col) for the triangular read by rows, * where
 *            T(row, col) = 3^col * (1 + (row mod 2))
 * Range:     u8 n <= 19, u16 n <= 75, u32 n <= 250, u64 n <= 900,
 *            u128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the row number r: the smallest r such that r*(r+1)/2 > n.
     * This is equivalent to r where cumulative(previous rows) <= n < cumulative(current row). */
    idx_t lo = 0;
    idx_t hi = n;
    idx_t r = 0;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        /* Use 128‑bit arithmetic to avoid overflow when computing the triangular number. */
        __uint128_t tri = (__uint128_t)mid * (mid + 1) / 2;
        if (tri <= n) {
            r = mid;
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }

    /* Count of terms before row r. */
    __uint128_t cum128 = (__uint128_t)r * (r + 1) / 2;
    idx_t cum = (idx_t)cum128;

    /* Column index within the row. */
    idx_t k = n - cum;

    /* Compute 3^k */
    seq_t value = 1;
    for (idx_t i = 0; i < k; ++i) {
        value *= 3;
    }

    /* Multiply by (1 + (r mod 2)) – 1 if r is even, 2 if r is odd. */
    if (r & 1U) {
        value *= 2;
    }
    /* If r is even we multiply by 1; no action needed. */

    return value;
}
