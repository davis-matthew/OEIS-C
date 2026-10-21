/*
 * A080251  Paired decomposition of tetrahedral numbers A000292 arranged as
 *          number triangle.
 * https://oeis.org/A080251
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the flattened triangle T(r,k) where T(r,k) =
 *            floor((k+2)/2) * (r - k + floor((k+3)/2)). For a given index n
 *            (offset 0), let r be the row such that r*(r+1)/2 <= n <
 *            (r+1)*(r+2)/2 and k = n - r*(r+1)/2
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Integer square root for unsigned 64‑bit values */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = n, ans = 0;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (mid != 0 && mid > n / mid) {
            hi = mid - 1;
        } else {
            ans = mid;
            lo = mid + 1;
        }
    }
    return ans;
}

/* Determine the row r for a zero‑based index idx */
static inline idx_t row_of(idx_t idx)
{
    /* Compute floor(sqrt(1 + 8*idx)) safely */
    idx_t sq = isqrt_u64(1 + 8 * idx);
    return (sq - 1) >> 1; /* r = floor((sqrt(1+8i)-1)/2) */
}

seq_t a(idx_t n)
{
    idx_t r = row_of(n);
    idx_t prev_sum = r * (r + 1) / 2;   /* sum of lengths of rows before r */
    idx_t k = n - prev_sum;             /* column within row r */

    seq_t part1 = (seq_t)((k + 2) >> 1);                     /* floor((k+2)/2) */
    seq_t part2 = (seq_t)r - (seq_t)k + (seq_t)((k + 3) >> 1); /* r - k + floor((k+3)/2) */

    return part1 * part2;
}
