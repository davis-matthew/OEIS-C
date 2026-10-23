/*
 * A132111  Triangle read by rows: T(n,k) = n^2 + k*n + k^2, 0 <= k <= n.
 * https://oeis.org/A132111
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is the n-th element of the triangle read row‑wise where
 *            T(r,c) = r² + r·c + c². For each n≥0 we find the row r and
 *            column k such that n = r·(r+1)/2 + k (0 ≤ k ≤ r), then compute
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*              a(n) = r² + r·k + k². */

seq_t a(idx_t n)
{
    /* Find the row r: the smallest r with r·(r+1)/2 > n. */
    idx_t r = 0;
    while ((unsigned __int128)r * (r + 1) / 2 <= n) {
        ++r;
    }
    --r;                      /* now r satisfies r·(r+1)/2 ≤ n < (r+1)(r+2)/2 */

    /* Compute column k. */
    idx_t prev = (idx_t)(((unsigned __int128)r * (r + 1) / 2));
    idx_t k = n - prev;

    /* Compute the triangle entry. */
    seq_t row = (seq_t)r;
    seq_t col = (seq_t)k;
    return row * row + col * row + col * col;
}
