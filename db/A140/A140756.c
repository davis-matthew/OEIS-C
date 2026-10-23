/*
 * A140756  Count up to k sequence with alternating signs (k always positive).
 * https://oeis.org/A140756
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign, tabl
 * Formula:   a(n) = (-1)^(r-k) * k where r is the smallest integer with
 *            r(r+1)/2 ≥ n and k = n - (r(r-1)/2) + 1 (the column number in
 *            row r)
 * Range:     i8 n <= 5000, i16 n <= 5000, i32 n <= 5000, i64 n <= 5000,
 *            i128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Find the triangular row r that contains index n.
   The row r satisfies r(r+1)/2 >= n and (r-1)r/2 < n. */
static inline idx_t find_row(idx_t n)
{
    /* Exponential search to find an upper bound */
    idx_t low = 1;
    idx_t high = 1;
    while (high * (high + 1) / 2 < n)
        high <<= 1;

    /* Binary search within [low, high] */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (mid * (mid + 1) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    idx_t r = find_row(n);                      /* row number          */
    idx_t start = r * (r - 1) / 2 + 1;           /* starting index of row r */
    idx_t k = n - start + 1;                    /* column number (1‑based) */

    seq_t val = (seq_t)k;
    /* sign depends on parity of r-k */
    if ((r - k) & 1)
        return -val;
    return val;
}
