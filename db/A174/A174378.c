/*
 * A174378  Triangle T(n, k) = n!*q^k/(n-k)! if floor(n/2) > k-1 otherwise
 *          n!*q^(n-k)/k!, with q = 4, read by rows.
 * https://oeis.org/A174378
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is the nth element of the triangle T(n,k) read * row‑by‑row
 *            (left to right), where * * T(n,k) = n!·4^k/(n−k)! if k <= ⌊n/2⌋
 *            * = n!·4^(n−k)/k! otherwise * * k is determined from the global
 *            index n by locating the row * that contains it. The row index r
 *            satisfies * * r(r+1)/2 <= n < (r+1)(r+2)...
 * Range:     u8 n <= 16, u16 n <= 39, u32 n <= 96, u64 n <= 263,
 *            u128 n <= 759, big any n
 * Helpers:   pow_4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_4.h"

static idx_t row_of_index(idx_t idx)
{
    idx_t low = 0;
    idx_t high = 0;

    /* Find a high bound where cumulative count exceeds idx */
    while (1) {
        __uint128_t tri = (__uint128_t)(high + 1) * (high + 2) / 2;
        if (tri > idx) break;
        if (high == 0) high = 1;
        else high <<= 1;
    }

    /* Binary search for smallest high with tri>idx */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        __uint128_t tri_mid = (__uint128_t)(mid + 1) * (mid + 2) / 2;
        if (tri_mid <= idx)
            low = mid + 1;
        else
            high = mid;
    }
    return high;
}

seq_t a(idx_t n)
{
    /* Locate the row r that contains offset n */
    idx_t r = row_of_index(n);

    /* Number of elements before this row: r*(r+1)/2 */
    idx_t row_start = r * (r + 1) / 2;
    /* Position within the row */
    idx_t k = n - row_start;

    /* Decide which formula applies */
    seq_t result = 1;
    if (k <= r / 2) {
        /* first case: n! * 4^k / (n-k)! */
        /* multiply from (n-k+1) to n */
        for (idx_t i = r - k + 1; i <= r; ++i) {
            result *= (seq_t)i;
        }
        result *= pow_4(k);
    } else {
        /* second case: n! * 4^(n-k) / k! */
        /* multiply from (k+1) to n */
        for (idx_t i = k + 1; i <= r; ++i) {
            result *= (seq_t)i;
        }
        result *= pow_4(r - k);
    }
    return result;
}
