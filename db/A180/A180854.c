/*
 * A180854  Square array read by antidiagonals: T(m,n) is the Wiener index of
 *          the lollipop graph L(m,n) (m>=1, n>=1). L(m,n) is the graph
 *          obtained by joining the complete graph K_m to the path graph P_n
 *          by an edge. The Wiener index of a connected graph is the sum of
 *          distances between all unordered pairs of v...
 * https://oeis.org/A180854
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   For m,n ≥ 1, T(m,n) = n(n²‑7)/6 + m(m+n²+3n‑1)/2. The sequence
 *            a(n) is obtained by reading the square array T(m,n) along
 *            antidiagonals starting at (1,1). Hence a(1)=T(1,1), a(2)=T(1,2),
 *            a(3)=T(2,1), a(4)=T(1,3), …
 * Range:     u8 n <= 62, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the diagonal d such that d(d+1)/2 ≥ n. */
    idx_t d = 1;
    for (;; ++d) {
        idx_t tri = d * (d + 1) / 2;
        if (tri >= n)
            break;
    }

    /* Elements preceding diagonal d */
    idx_t before = (d - 1) * d / 2;
    /* 0‑based position within the diagonal */
    idx_t offset = n - before - 1;
    /* Convert to array coordinates (row = m, column = col) */
    idx_t m   = d - offset;
    idx_t col = offset + 1;

    /* Compute T(m,col) using signed 128‑bit arithmetic to avoid
       overflow and to handle the small negative intermediate for
       col = 1.  The final result is always non‑negative. */
    __int128 m128   = m;
    __int128 col128 = col;
    __int128 part1  = col128 * (col128 * col128 - 7) / 6;
    __int128 part2  = m128 * (m128 + col128 * col128 + 3 * col128 - 1) / 2;
    __int128 res128 = part1 + part2;

    return (seq_t)res128;
}
