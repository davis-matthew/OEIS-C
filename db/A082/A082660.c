/*
 * A082660  Number of ways n can be expressed as the sum of a square and a
 *          triangular number.
 * https://oeis.org/A082660
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # { (k,m) | n = k² + m(m+1)/2, k ≥ 0, m ≥ 1 }
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* integer square root of a 64‑bit unsigned integer (floor) */
static inline seq_t floor_sqrt_u64(idx_t x)
{
    idx_t lo = 0, hi = 1;
    /* find an upper bound */
    while ((unsigned __int128)hi * hi <= x)
        hi <<= 1;
    lo = hi >> 1;
    /* binary search */
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if ((unsigned __int128)mid * mid <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t m = 1;; ++m) {
        /* triangular number T_m = m(m+1)/2 */
        unsigned __int128 tri128 = (unsigned __int128)m * (m + 1) / 2;
        if (tri128 > n)
            break;
        idx_t rem = n - (idx_t)tri128;        /* n - T_m */
        seq_t root = floor_sqrt_u64(rem);     /* floor sqrt(rem) */
        if ((unsigned __int128)root * root == rem)
            ++count;
    }
    return count;
}
