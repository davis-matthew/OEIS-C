/*
 * A013627  Triangle of coefficients in expansion of (6+7*x)^n.
 * https://oeis.org/A013627
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, nonn, easy
 * Formula:   a(n) = C(r,k) * 6^(r-k) * 7^k, where r = floor((sqrt(8*n + 1) -
 *            1) / 2), k = n - r*(r+1)/2
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* Find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
    idx_t lo = 0, hi = 1;
    while (((__int128)hi * (hi + 1) / 2) <= n) hi <<= 1;
    while (lo < hi) {
        idx_t mid = (lo + hi) >> 1;
        if (((__int128)mid * (mid + 1) / 2) <= n)
            lo = mid + 1;
        else
            hi = mid;
    }
    idx_t row = lo - 1;
    idx_t start = (idx_t)((__int128)row * (row + 1) / 2);
    idx_t k = n - start;                    /* column index in this row */

    /* Compute binomial coefficient C(row, k). */
    seq_t binom = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i) {
        binom = binom * (seq_t)(row - k + i);
        binom = binom / (seq_t)i;
    }

    /* 6^(row-k) */
    seq_t pow6 = (seq_t)1;
    for (idx_t i = 0; i < row - k; ++i) {
        pow6 = pow6 * (seq_t)6;
    }

    /* 7^k */
    seq_t pow7 = (seq_t)1;
    for (idx_t i = 0; i < k; ++i) {
        pow7 = pow7 * (seq_t)7;
    }

    return binom * pow6 * pow7;
}
