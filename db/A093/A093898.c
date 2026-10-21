/*
 * A093898  Triangle read by rows: T(n,r) = n^r + r^n (1 <= r <= n).
 * https://oeis.org/A093898
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n-th term of the triangle T(i,j) = i^j + j^i, read
 *            row by row. The row index r satisfies (r‑1)·r/2 < n ≤ r·(r+1)/2,
 *            and the column index k is n – (r‑1)·r/2. The value is a(n) = r^k
 *            + k^r
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   pow_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_big.h"

/* find the row r such that r*(r+1)/2 ≥ n */
static idx_t find_row(idx_t n)
{
    idx_t low = 1;
    idx_t high = 1;
    /* grow high until the inequality holds */
    while ( ((__int128)high * (high + 1)) / 2 < n )
    {
        if (high > UINT64_MAX / 2) { high = UINT64_MAX; break; }
        high <<= 1;
    }
    while (low < high)
    {
        idx_t mid = low + ((high - low) >> 1);
        if ( ((__int128)mid * (mid + 1)) / 2 >= n )
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* locate row and column */
    idx_t r = find_row(n);
    __int128 prev = ((__int128)(r - 1) * r) / 2;
    idx_t k = n - (idx_t)prev;            /* 1 ≤ k ≤ r */

    /* compute the two powers and add them */
    seq_t p1 = pow_big((seq_t)r, k);
    seq_t p2 = pow_big((seq_t)k, r);
    return p1 + p2;
}
