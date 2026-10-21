/*
 * A240222  Rectangular array giving all start values M(n, k), k >= 1, for
 *          Collatz sequences following the pattern (udd)^(n-1) ud, n >= 1,
 *          read by antidiagonals.
 * https://oeis.org/A240222
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) is obtained by reading the array M(n,k)=1+2^(2n-1)*k by
 *            antidiagonals. For n≥1, let s be the smallest integer with
 *            s*(s+1)/2 ≥ n. Then m = n - s*(s-1)/2 (the row index in that
 *            antidiagonal) and k = s - m. The required term is a(n) = 1 +
 *            2^(2*m-1) * k
 * Range:     u8 n <= 18, u16 n <= 52, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the diagonal s such that n lies on it. */
    idx_t s = 1;
    idx_t tri = 1;          /* T_s = s*(s+1)/2 */
    while (tri < n) {
        ++s;
        tri += s;
    }
    /* tri now equals T_s.  Previous diagonal count: T_{s-1} = tri - s. */
    idx_t prev = tri - s;           /* T_{s-1} */
    idx_t offset = n - prev - 1;    /* zero‑based offset inside diagonal s */
    idx_t m = offset + 1;           /* row index (n' in the description) */
    idx_t k = s - m;                /* column index (k' in the description) */

    /* Compute 1 + 2^(2*m-1) * k */
    idx_t shift = 2 * m - 1;
    seq_t pow2 = (seq_t)1;
    for (idx_t i = 0; i < shift; ++i) {
        pow2 *= (seq_t)2;           /* repeated doubling;  unsigned overflow is defined */
    }
    return (seq_t)1 + pow2 * (seq_t)k;
}
