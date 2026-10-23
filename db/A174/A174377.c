/*
 * A174377  Triangle T(n, k) = n!*q^k/(n-k)! if floor(n/2) > k-1 otherwise
 *          n!*q^(n-k)/k!, with q = 3, read by rows.
 * https://oeis.org/A174377
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = product_{i=0}^{min(k,r-k)-1} ((r-i)*3) where r =
 *            floor((sqrt(8n+1)-1)/2) and k = n - r(r+1)/2 (the sequence
 *            A174377 read row‑by‑row)
 * Range:     u8 n <= 22, u16 n <= 39, u32 n <= 111, u64 n <= 311,
 *            u128 n <= 839, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the closed formula for the triangle elements
   without needing external helpers. */

static inline idx_t isqrt_u64(idx_t x)
{
    /* Integer square root: largest r with r*r <= x */
    if (x < 2) return x;
    idx_t low = 0, high = (x >> 1) + 1;   /* high >= sqrt(x) for x>=2 */
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if (mid <= x / mid)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* Determine row index r from n by solving r(r+1)/2 <= n < (r+1)(r+2)/2 */
    idx_t s = isqrt_u64(8 * n + 1);   /* sqrt(8n+1)  */
    idx_t r = (s - 1) / 2;            /* floor((sqrt(8n+1)-1)/2) */

    /* Position k in the row (0‑based) */
    idx_t prev = r * (r + 1) / 2;
    idx_t k = n - prev;

    /* Number of multiplicative terms */
    idx_t m = (k <= r - k) ? k : (r - k);

    /* Compute the product ((r-i)*3) for i=0..m-1 */
    seq_t res = (seq_t)1;
    for (idx_t i = 0; i < m; ++i) {
        res *= (seq_t)((seq_t)(r - i) * (seq_t)3);
    }
    return res;
}
