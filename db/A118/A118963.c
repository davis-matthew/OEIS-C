/*
 * A118963  Triangle read by rows: T(n,k) is the number of Grand Dyck paths of
 *          semilength n that have k double rises (n >= 1, k >= 0).
 * https://oeis.org/A118963
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(m,k) with m = minimal integer with m(m+1)/2 >= n and k
 *            = n - m(m-1)/2 - 1 where T(m,k) = ((m+1)/m) * binomial(m,k) *
 *            binomial(m,k+1)
 * Range:     u8 n <= 17, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sequencea A118963 for index n (1‑based). */
seq_t a(idx_t n)
{
    /* find row m such that m(m+1)/2 >= n */
    idx_t lo = 1, hi = 1;
    while ( (unsigned __int128)hi * (hi + 1) / 2 < n ) {
        hi <<= 1;                /* grow upper bound exponentially */
    }
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        unsigned __int128 prod = (unsigned __int128)mid * (mid + 1) / 2;
        if (prod >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t m = lo;                                     /* row number */

    /* first index of this row */
    idx_t prev_sum = (m - 1) * m / 2;
    idx_t k = n - prev_sum - 1;                       /* 0‑based within row */

    /* compute T(m,k) */
    if (k == 0)
        return (seq_t)(m + 1);

    seq_t val = (seq_t)(m + 1);                       /* T(m,0) */
    for (idx_t i = 0; i < k; ++i) {
        /* val = val * (m-i) * (m-i-1) / ((i+1)*(i+2))    */
        unsigned __int128 numerator   = (unsigned __int128)val * (m - i) * (m - i - 1);
        unsigned __int128 denominator = (unsigned __int128)(i + 1) * (i + 2);
        val = (seq_t)(numerator / denominator);
    }
    return val;
}
