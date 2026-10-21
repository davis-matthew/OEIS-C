/*
 * A095891  Triangle read by rows: T(n,k) = (n-k+1)^(n-k+1), n>=1, 1<=k<=n.
 * https://oeis.org/A095891
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) is the (n‑th) term of the triangle read by rows where
 *            T(r,c) = (r‑c+1)^(r‑c+1) for 1 ≤ c ≤ r. The row r is the
 *            smallest integer with r*(r+1)/2 ≥ n, and c = n - (r‑1)*r/2. Then
 *            a(n) = (r‑c+1)^(r‑c+1)
 * Range:     u8 n <= 6, u16 n <= 21, u32 n <= 45, u64 n <= 120, u128 n <= 351,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* safety for out‑of‑range input */

    /* Find the row r such that r*(r+1)/2 >= n */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n) r++;

    /* Position within that row */
    idx_t prev_total = (r - 1) * r / 2;
    idx_t c = n - prev_total;          /* 1-based column index */
    idx_t m = r - c + 1;                /* base / exponent */

    /* Compute m^m via exponentiation by squaring */
    seq_t base = (seq_t)m;
    seq_t res = 1;
    idx_t exp = m;
    while (exp) {
        if (exp & 1) res *= base;
        exp >>= 1;
        if (exp) base *= base;
    }
    return res;
}
