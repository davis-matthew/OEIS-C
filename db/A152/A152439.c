/*
 * A152439  Irregular triangle T(n, k) = (n-3)*(k-n)*(k-n-2) - (2*n-k)*(k-2),
 *          with 0 <= k <= 2*n.
 * https://oeis.org/A152439
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabf, sign
 * Formula:   a(n) = T(r, k) where r = floor(sqrt(n)), k = n - r^2, * and
 *            T(r,k) = (r-3)*(k-r)*(k-r-2) - (2*r - k)*(k-2)
 * Range:     i8 n <= 35, i16 n <= 1088, i32 n <= 2600, i64 n <= 2600,
 *            i128 n <= 2600, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline idx_t floor_isqrt_u64(idx_t x)
{
    /* binary search for floor(sqrt(x)) within [0, 0xFFFFFFFF] */
    idx_t low = 0;
    idx_t high = 0xFFFFFFFFULL; /* 2^32‑1 > sqrt(2^64‑1) */
    idx_t ans = 0;
    while (low <= high)
    {
        idx_t mid = (low + high) >> 1;
        unsigned __int128 sq = (unsigned __int128)mid * (unsigned __int128)mid;
        if (sq <= (unsigned __int128)x)
        {
            ans = mid;
            low = mid + 1;
        }
        else
        {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    /* Determine row r and column k for the flattened triangle. */
    idx_t r = floor_isqrt_u64(n);          /* r^2 <= n < (r+1)^2 */
    idx_t k = n - r * r;                   /* 0 <= k <= 2*r */

    /* Convert to seq_t for arithmetic. */
    seq_t rn = (seq_t)r;
    seq_t kn = (seq_t)k;

    /* Compute T(r,k). */
    seq_t term1 = (rn - (seq_t)3) * (kn - rn) * (kn - rn - (seq_t)2);
    seq_t term2 = ((seq_t)(2 * r) - kn) * (kn - (seq_t)2);
    return term1 - term2;
}
