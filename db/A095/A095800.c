/*
 * A095800  Triangle T(n,k) = abs( k *( (2*n+1)*(-1)^(n+k)+2*k-1) /4 ) read by
 *          rows, 1<=k<=n.
 * https://oeis.org/A095800
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = |k * ((2r+1)*(-1)^(r+k)+2k-1)/4| where r =
 *            ceil((sqrt(8n+1)-1)/2) (row number) and k = n - r(r-1)/2
 *            (position in that row)
 * Range:     u8 n <= 35, u16 n <= 7875, u32 n <= 7875, u64 n <= 7875,
 *            u128 n <= 7875, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Compute floor(sqrt(x)) for an unsigned 64‑bit integer using
   binary search with a 128‑bit intermediate to avoid overflow. */
static inline idx_t _floor_isqrt_u64(idx_t x)
{
    idx_t lo = 0, hi = x;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        __int128 sq = (__int128)mid * mid;
        if (sq <= x)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

/* Find the row r and column k that contain the n‑th element
   of the triangle, where rows start at 1.  The function
   sets *row and *col respectively. */
static inline void _row_col_from_index(idx_t n, idx_t *row, idx_t *col)
{
    /* Compute r = ceil((sqrt(8n+1)-1)/2) */
    idx_t disc = 1 + 8 * n;
    idx_t sqrt_disc = _floor_isqrt_u64(disc);
    idx_t r = (sqrt_disc - 1) / 2;
    /* If r(r+1)/2 < n, we need the next row */
    __int128 triangular = (__int128)r * (r + 1) / 2;
    if (triangular < n)
        r += 1;
    *row = r;
    /* k = n - r(r-1)/2 */
    __int128 prev_triangular = (__int128)r * (r - 1) / 2;
    *col = (idx_t)(n - prev_triangular);
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* Undefined for n = 0, but guard for safety */

    idx_t r, k;
    _row_col_from_index(n, &r, &k);

    /* Compute B = 2*r + 1 */
    seq_t B = (seq_t)2 * (seq_t)r + 1;

    /* val2 = 2*k - 1 */
    seq_t val2 = (seq_t)2 * (seq_t)k;
    val2 -= 1;

    /* parity of (r + k) */
    bool even = ((r + k) & 1) == 0;
    seq_t inside_abs;
    if (even) {
        inside_abs = B + val2;          /* (2r+1) + (2k-1) */
    } else {
        inside_abs = B - val2;          /* (2r+1) - (2k-1)  (positive) */
    }

    /* product_abs = k * inside_abs */
    seq_t product_abs = (seq_t)k * inside_abs;

    /* Final term: divide by 4 */
    return product_abs / 4;
}
