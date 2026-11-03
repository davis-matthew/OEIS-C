/*
 * A062169  Triangle T(n, k) = k! mod n for n >= 1, 1 <= k <= n.
 * https://oeis.org/A062169
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r, k) where r is the unique integer with r(r-1)/2 < n
 *            <= r(r+1)/2 (i.e., the row of the triangle) and k = n - r(r-1)/2
 *            is the column in that row. T(r, k) = k! mod r. The function
 *            implements this definition
 * Range:     u8 n <= 1275, u16 n <= 1275, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Validated: standard, pass, 118 terms through n = 1170 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t col_in_row(idx_t n, idx_t r) {
    /* k = n - r(r-1)/2 */
    return n - r * (r - 1) / 2;
}

static idx_t row_of_n(idx_t n) {
    /* Find the smallest r such that r*(r+1)/2 >= n.
       Binary search using 128‑bit arithmetic to avoid overflow. */
    idx_t low = 1;
    idx_t high = 1;
    while ((unsigned __int128)high * (high + 1) / 2 < n) {
        high <<= 1;                 /* grow until sufficient */
    }
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ((unsigned __int128)mid * (mid + 1) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;                 /* safety, though n>=1 per spec */

    idx_t r = row_of_n(n);               /* row number */
    idx_t k = col_in_row(n, r);          /* column number */

    seq_t mod = (seq_t)r;
    unsigned __int128 mod128 = (unsigned __int128)mod;

    seq_t prod = 1 % mod;                /* initial factorial value */
    for (idx_t j = 1; j <= k; ++j) {
        unsigned __int128 tmp = (unsigned __int128)prod * (unsigned __int128)j;
        prod = (seq_t)(tmp % mod128);
    }
    return prod;
}
