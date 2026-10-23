/*
 * A194291  Triangular array: g(n,k)=number of fractional parts (i*sqrt(3)) in
 *          interval [(k-1)/n, k/n], for 1<=i<=n^2, 1<=k<=n.
 * https://oeis.org/A194291
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the g(row, col) of the triangular array * where g(n,k) =
 *            number of indices i (1 ≤ i ≤ n²) such that * fractional part of
 *            i*sqrt(3) lies in the interval [(k-1)/n, k/n]. * row = minimal r
 *            with r(r+1)/2 ≥ n, * col = n - r(r-1)/2. g(row,col) is returned
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Integer square root: floor(sqrt(x)) for unsigned 128‑bit x. */
static inline idx_t isqrt_u128(unsigned __int128 x)
{
    idx_t lo = 0;
    idx_t hi = (idx_t)-1;                     /* maximum 64‑bit value */
    while (lo <= hi)
    {
        idx_t mid = lo + (hi - lo) / 2;
        unsigned __int128 mid2 = (unsigned __int128)mid * mid;
        if (mid2 <= x)
        {
            lo = mid + 1;
        }
        else
        {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return hi;
}

/* Main function required by the build. */
seq_t a(idx_t n)
{
    /* find the row r such that r(r+1)/2 ≥ n (triangular numbers) */
    unsigned __int128 tmp = (unsigned __int128)8 * n + 1;
    idx_t s = isqrt_u128(tmp);
    idx_t r = (s - 1) / 2;
    if ((unsigned __int128)r * (r + 1) / 2 < n) r++;

    /* column (1‑based) within that row */
    idx_t prev_tri = (unsigned __int128)(r - 1) * r / 2;
    idx_t col = n - prev_tri;          /* 1‑based */

    /* Allocate histogram for counts in this row */
    seq_t *cnt = (seq_t *)malloc((size_t)r * sizeof(seq_t));
    if (!cnt) return 0;                /* memory failure – return 0 */
    for (idx_t i = 0; i < r; ++i) cnt[i] = 0;

    /* Count for each i = 1 .. r² */
    unsigned __int128 max_i = (unsigned __int128)r * r;
    for (idx_t ii = 1; ii <= max_i; ++ii)
    {
        /* floor(i * sqrt(3))        */
        unsigned __int128 a_val = (unsigned __int128)3 * ii * ii;
        idx_t floor_i = isqrt_u128(a_val);

        /* floor(r * i * sqrt(3))    */
        unsigned __int128 prod = (unsigned __int128)r * ii;
        unsigned __int128 b_val = (unsigned __int128)3 * prod * prod;
        idx_t floor_ri = isqrt_u128(b_val);

        /* bin index = floor(r*i*sqrt(3)) - r*floor(i*sqrt(3))  */
        idx_t bin = floor_ri - r * floor_i;
        ++cnt[bin];
    }

    seq_t result = cnt[col - 1];
    free(cnt);
    return result;
}
