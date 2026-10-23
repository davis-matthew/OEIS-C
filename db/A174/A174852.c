/*
 * A174852  Triangle T(n,k) read by rows. T(1,1)=1, n>1 T(n,1)=A049240, k>1
 *          T(n,k)=A051731.
 * https://oeis.org/A174852
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 1 unless the triangle coordinate (row,column) * satisfies
 *            one of: * (row>1 , column=1 and row is a perfect square) -> 0
 *            (A049240) * (column>1 and column does not divide row) -> 0
 *            (A051731) * Otherwise a(n)=1. That is, the sequence is the
 *            triangular * read‑by‑row of T(n,k) with T(1,1)=1, fi...
 * Range:     u8 n <= 44, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t isqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = (idx_t)1ULL << 32; /* 2^32 >= sqrt(2^64) */
    while (lo < hi)
    {
        idx_t mid = (lo + hi + 1) >> 1;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* Find the row r such that (r-1)*r/2 < n <= r*(r+1)/2 */
    idx_t low = 1, high = n;
    while (low < high)
    {
        idx_t mid = (low + high) >> 1;
        seq_t tri = ((seq_t)mid * (((seq_t)mid) + 1)) / 2;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* Column index within the row (1‑based) */
    seq_t prev_tri_seq = ((seq_t)(r - 1) * (seq_t)r) / 2;
    idx_t prev_tri = (idx_t)prev_tri_seq;        /* fits in idx_t */
    idx_t c = n - prev_tri;                      /* 1‑based column */

    if (c == 1)                /* first column – A049240 */
    {
        if (r == 1)
            return (seq_t)1;
        idx_t sq = isqrt_u64(r);
        if (sq * sq == r && r > 1)
            return (seq_t)0;
        return (seq_t)1;
    }
    else                        /* k > 1 – A051731 */
    {
        return (seq_t)(((r % c) == 0) ? 1 : 0);
    }
}
