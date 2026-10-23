/*
 * A154614  Triangle read by rows where T(m,n) = m*n + m + n - 1, 1<=n<=m.
 * https://oeis.org/A154614
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = m*j + m + j - 1 with * m = ceil((-1 + sqrt(1+8n))/2) (row
 *            index) * j = n - (m-1)m/2 (position in the row)
 * Range:     u8 n <= 134, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline idx_t floor_sqrt_u128(__uint128_t x)
{
    idx_t low = 0, high = UINT64_MAX;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq == x)
            return mid;
        if (sq < x)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

seq_t a(idx_t n)
{
    /* Find row m such that (m-1)m/2 < n <= m(m+1)/2 */
    __uint128_t val = (__uint128_t)8 * n + 1;
    idx_t sqrtX = floor_sqrt_u128(val);
    idx_t s = (sqrtX - 1) / 2;                       /* floor value of (-1+sqrt(...))/2 */
    idx_t t = (idx_t)(((__uint128_t)s * (s + 1)) / 2);  /* triangular number s(s+1)/2 */
    idx_t m = (t < n) ? s + 1 : s;                    /* minimal m with m(m+1)/2 >= n */

    /* Position j in row m */
    idx_t t_prev = (idx_t)(((__uint128_t)(m - 1) * m) / 2);
    idx_t j = n - t_prev;                            /* 1 <= j <= m */

    /* Compute T(m,j) = m*j + m + j - 1 */
    seq_t result = (seq_t)m * (seq_t)j + (seq_t)m + (seq_t)j - (seq_t)1;
    return result;
}
