/*
 * A194313  Triangular array: g(n,k)=number of fractional parts (i*sqrt(6)) in
 *          interval [(k-1)/n, k/n], for 1<=i<=n, 1<=k<=n.
 * https://oeis.org/A194313
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) is the nth element of the triangular array
    g(r,k) = # { 1≤i≤r : frac(i*√6) ∈ [(k-1)/r , k/r) } .
    For a global index n we find the row r satisfying
          r(r+1)/2 ≥ n  > (r-1)r/2
    then c = n – (r-1)r/2 is the column.
    g(r,c) is computed by counting i with
          floor(r*i*√6) – r*floor(i*√6) = c-1 .
    floor(t*√6) is obtained exactly by
          floor(√(6*t²)) using an integer square‑root.       */

#include <stdint.h>

/* integer square root: floor(sqrt(v)) for v≤2^64-1 */
static idx_t isqrt_u64(idx_t v)
{
    /* binary search on [0, 2^32] – 2^32² fits in 128 bits */
    idx_t lo = 0;
    idx_t hi = (1ULL << 32);                /* 4,294,967,296 */
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;      /* +1 for ceiling bias */
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= v)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* find row r such that r(r+1)/2 >= n */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n)
        ++r;

    /* column index c in this row (1‑based) */
    idx_t prev = ((r - 1) * r) / 2;
    idx_t c = n - prev;                /* 1 <= c <= r */

    /* target value for the floor difference: c-1 */
    const idx_t target = c - 1;

    /* count of i with floor(r*i*√6) – r*floor(i*√6) = target */
    idx_t cnt = 0;
    for (idx_t i = 1; i <= r; ++i) {
        idx_t i2 = i * i;                 /* i², fits in 64 bits for r≤141 */
        idx_t r_i = r * i;                /* (r*i)² also fits safely */
        idx_t floor_i  = isqrt_u64(6 * i2);           /* floor(i*√6)  */
        idx_t floor_r = isqrt_u64(6 * r_i * r_i);    /* floor(r*i*√6) */
        idx_t diff = floor_r - r * floor_i;          /* floor(r*frac)*/
        if (diff == target)
            ++cnt;
    }
    return (seq_t)cnt;
}
