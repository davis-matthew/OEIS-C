/*
 * A105725  Triangle read by rows: T(n,k)=(n+k)!/k! (0<=k<=n-1; n>=1).
 * https://oeis.org/A105725
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (r + k)! / k! where r = smallest integer with r(r+1)/2 >=
 *            n, k = n - r(r-1)/2 - 1 (0 ≤ k < r)
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset is 1, not used */
    idx_t row = 1;
    idx_t tri = 1;                       /* triangular number Σ1..row */
    while (tri < n) {
        ++row;
        tri += row;
    }
    idx_t prev_tri = tri - row;          /* Σ1..(row-1) */
    idx_t offset = n - prev_tri;         /* 1‑based position inside the row */
    idx_t k = offset - 1;                /* 0‑based column index */

    seq_t val = 1;
    for (idx_t i = 1; i <= row; ++i) {
        val *= (seq_t)(k + i);
    }
    return val;
}
