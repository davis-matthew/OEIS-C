/*
 * A124846  Triangle read by rows: T(n,k) = (2 - (-1)^k)*binomial(n,k) (0 <= k
 *          <= n).
 * https://oeis.org/A124846
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = flatten of triangle T(m,k) where T(m,k) = (2 -
 *            (-1)^k)*binomial(m,k)
 * Range:     u8 n <= 39, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Search for the row r such that start(r) <= n < start(r) + (r+1) */
    idx_t low = 0;
    idx_t high = n + 1;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        __int128 tri = (__int128)mid * (mid + 1) / 2;
        if (tri <= n)
            low = mid + 1;
        else
            high = mid;
    }
    idx_t r = low - 1;

    /* Starting index of row r */
    idx_t start = (idx_t)((__int128)r * (r + 1) / 2);
    /* Position within the row */
    idx_t k = n - start;

    /* Compute binomial coefficient and apply multiplier */
    seq_t bin = binomial(r, k);
    seq_t mult = (k & 1) ? (seq_t)3 : (seq_t)1;

    return bin * mult;
}
