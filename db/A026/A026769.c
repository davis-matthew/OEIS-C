/*
 * A026769  Triangular array T read by rows: T(n,0)=T(n,n)=1 for n >= 0;
 *          T(2,1)=2; for n >= 3 and 1<=k<=n-1, T(n,k) = T(n-1,k-1) +
 *          T(n-2,k-1) + T(n-1,k) if 1<=k<=(n-1)/2, else T(n,k) = T(n-1,k-1) +
 *          T(n-1,k).
 * https://oeis.org/A026769
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 141, u32 n <= 446, u64 n <= 1562,
 *            u128 n <= 5150, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A026769:  T(n,0)=T(n,n)=1,  T(2,1)=2
   For n≥3, 1≤k≤n−1:
     if k≤(n−1)/2:  T(n,k)=T(n−1,k−1)+T(n−2,k−1)+T(n−1,k)
     else          :  T(n,k)=T(n−1,k−1)+T(n−1,k)
   a(n)=T(r,c) where r is the row with r(r+1)/2≤n<(r+1)(r+2)/2
   and c=n−r(r+1)/2. */

#include <stdlib.h>
#include <stdint.h>

/* Integer square root – floor(sqrt(x)) for 64‑bit unsigned x */
static inline idx_t floor_isqrt_u64(idx_t x)
{
    if (x < 2) return x;
    idx_t lo = 0, hi = ((idx_t)1ULL << 32);
    while (lo < hi) {
        idx_t mid = lo + (hi - lo + 1) / 2;
        if ((unsigned __int128)mid * mid <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* Convert (row,col) to a flat array index */
static inline idx_t index_of(idx_t r, idx_t c)
{
    return (idx_t)(((unsigned __int128)r * (r + 1)) / 2 + c);
}

/* Flat storage of the triangular array */
static seq_t *tri = NULL;
static idx_t cur_rows = 0;   /* number of rows already computed */

/* Ensure rows 0 … r_needed are computed and stored in tri */
static void ensure_rows(idx_t r_needed)
{
    if (cur_rows > r_needed) return;

    /* size needed up to row r_needed inclusive */
    idx_t new_total =
        (idx_t)(((unsigned __int128)(r_needed + 1) * (r_needed + 2)) / 2);

    tri = (seq_t *)realloc(tri, (size_t)new_total * sizeof(seq_t));

    for (idx_t r = cur_rows; r <= r_needed; ++r) {
        idx_t base = (idx_t)(((unsigned __int128)r * (r + 1)) / 2);
        for (idx_t c = 0; c <= r; ++c) {
            seq_t val;
            if (c == 0 || c == r) {
                val = (seq_t)1;
            } else if (r == 2 && c == 1) {
                val = (seq_t)2;
            } else {
                idx_t t1_idx = index_of(r - 1, c - 1);
                idx_t t2_idx = index_of(r - 1, c);
                if (r >= 3 && c <= (r - 1) / 2) {
                    idx_t t3_idx = index_of(r - 2, c - 1);
                    val = tri[t1_idx] + tri[t2_idx] + tri[t3_idx];
                } else {
                    val = tri[t1_idx] + tri[t2_idx];
                }
            }
            tri[base + c] = val;
        }
    }
    cur_rows = r_needed + 1;
}

seq_t a(idx_t n)
{
    /* find row r with r(r+1)/2 ≤ n < (r+1)(r+2)/2 */
    idx_t d = 8 * n + 1;
    idx_t sqrt_d = floor_isqrt_u64(d);
    idx_t r = (sqrt_d - 1) / 2;
    idx_t base = (idx_t)(((unsigned __int128)r * (r + 1)) / 2);
    idx_t c = n - base;
    ensure_rows(r);
    return tri[base + c];
}
