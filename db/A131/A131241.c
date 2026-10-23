/*
 * A131241  3*A046854 - 2*I.
 * https://oeis.org/A131241
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   * a(n) = 3 * binomial( floor((r + c) / 2), c ) - 2 * δ(c, r) *
 *            where n = r*(r+1)/2 + c (0 ≤ c ≤ r) * (δ is the Kronecker delta,
 *            1 if c = r, else 0)
 * Range:     u8 n <= 54, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Helper: integer square root of a 128‑bit unsigned value
 * returned value is the floor of the sqrt. */
static inline idx_t isqrt_u128(unsigned __int128 val)
{
    idx_t lo = 0, hi = ~(idx_t)0, ans = 0;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= val) {
            ans = mid;
            lo  = mid + 1;
        } else {
            hi  = (mid == 0) ? 0 : mid - 1;
        }
    }
    return ans;
}

/* Helper: binomial(m, k) as an exact integer in seq_t. */
static inline seq_t binom(idx_t m, idx_t k)
{
    if (k > m - k)    /* use symmetry */
        k = m - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        unsigned __int128 temp = (unsigned __int128)res * (m - k + i);
        temp /= i;
        res = (seq_t)temp;
    }
    return res;
}

/* Main SEQUENCE function. */
seq_t a(idx_t n)
{
    /* find (r, c)  with  n = r*(r+1)/2 + c,  0 ≤ c ≤ r */
    unsigned __int128 val = (unsigned __int128)8 * n + 1;      /* 8*n + 1 */
    idx_t sqrt_val = isqrt_u128(val);                         /* floor(sqrt) */
    idx_t r = (sqrt_val - 1) / 2;
    idx_t row_start = r * (r + 1) / 2;
    idx_t c = n - row_start;

    /* compute binomial part */
    idx_t m = (r + c) / 2;
    seq_t bin = binom(m, c);

    seq_t result = (seq_t)3 * bin;
    if (c == r)            /* diagonal entry */
        result -= (seq_t)2;

    return result;
}
