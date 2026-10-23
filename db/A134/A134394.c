/*
 * A134394  Triangle T(n,k) = Sum_{j=k..n} A077028(j,k), read by rows.
 * https://oeis.org/A134394
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,c) with r = row index (0‑based), c = column index
 *            (0‑based), where T(r,c) = (c - r - 1)*(c*(c - r) - 2)/2. The
 *            sequence a(n) is the triangle read by rows with offset 1
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   triangular_cumulative, triangular_preceding
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/triangular_cumulative.h"
#include "../utils/triangular_preceding.h"

seq_t a(idx_t n)
{
    /* n >= 1, find row r such that triangular_preceding(r) < n <= triangular_cumulative(r) */
    idx_t low = 0, high = n;          /* high is always sufficient */
    while (low < high) {
        idx_t mid = low + (high - low) / 2U;
        if (triangular_cumulative(mid) >= n)
            high = mid;
        else
            low = mid + 1U;
    }
    idx_t r = low;
    idx_t preceding = triangular_preceding(r);
    idx_t c = n - preceding - 1U;        /* column index, 0‑based */

    /* Compute T(r,c) = (c - r - 1)*(c*(c - r) - 2)/2 using signed 128‑bit for safety */
    __int128 a_val = (__int128)c - (__int128)r - 1;
    __int128 b_val = (__int128)c * ((__int128)c - (__int128)r) - 2;
    __int128 prod  = a_val * b_val;
    prod /= 2;
    return (seq_t)prod;
}
