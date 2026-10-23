/*
 * A119307  Triangle read by rows: T(n, k) = Sum_{j=0..n} C(j, k)*C(j, n - k).
 * https://oeis.org/A119307
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = flattened triangle T(n,k) where T(m,k) = Σ_{j=0..m}
 *            C(j,k)*C(j,m-k) The index n is converted to row m and column k
 *            by solving m*(m+1)/2 <= n < (m+1)*(m+2)/2
 * Range:     u8 n <= 22, u16 n <= 59, u32 n <= 197, u64 n <= 645,
 *            u128 n <= 2375, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <limits.h>
#include "../utils/binomial.h"

static idx_t _row_from_index(idx_t n)
{
    /* Find the largest m such that m*(m+1)/2 <= n */
    idx_t low  = 0;
    idx_t high = 1;

    /* Quickly increase the upper bound */
    while (1) {
        unsigned __int128 tri = (unsigned __int128)high * (high + 1) / 2;
        if (tri > n || high == UINT64_MAX) break;
        if (high == UINT64_MAX / 2) { high = UINT64_MAX; break; }
        high <<= 1;
    }

    /* Binary search between low and high */
    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri <= n)
            low = mid;
        else
            high = mid;
    }
    return low;
}

seq_t a(idx_t n)
{
    idx_t row = _row_from_index(n);
    idx_t col = n - row * (row + 1) / 2;

    /* Compute T(row, col) = Σ_{j=max(col,row-col)}^{row} C(j,col)*C(j,row-col) */
    seq_t sum = (seq_t)0;
    idx_t start = col > row - col ? col : row - col;
    for (idx_t j = start; j <= row; ++j) {
        seq_t c1 = binomial(j, col);
        seq_t c2 = binomial(j, row - col);
        sum += c1 * c2;
    }
    return sum;
}
