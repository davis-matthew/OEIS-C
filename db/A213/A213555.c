/*
 * A213555  Rectangular array: (row n) = b**c, where b(h) = h^3, c(h) = n-1+h,
 *          n>=1, h>=1, and ** = convolution.
 * https://oeis.org/A213555
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = sum_{h=1}^{c} h^3 * (r + c - h), where (r,c) are the row
 *            and column indices obtained from the falling‑antidiagonal
 *            enumeration of the rectangular array
 * Range:     u8 n <= 10, u16 n <= 120, u32 n <= 1830, u64 n <= 1830,
 *            u128 n <= 1830, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* safety for out‑of‑range input */

    /* Find the diagonal index s such that (s+1)(s+2)/2 ≥ n */
    idx_t s = 0;
    while ((s + 1) * (s + 2) / 2 < n)
        ++s;

    /* Count of elements before this diagonal */
    idx_t prev = s * (s + 1) / 2;
    idx_t pos  = n - prev - 1;          /* zero‑based position in the diagonal */
    idx_t r    = pos + 1;               /* row number   */
    idx_t c    = (s - pos) + 1;         /* column number */

    /* Compute sums of h^3 and h^4 for h = 1 .. c */
    seq_t sum3 = 0;
    seq_t sum4 = 0;
    for (idx_t h = 1; h <= c; ++h) {
        seq_t hseq = (seq_t)h;
        sum3 += hseq * hseq * hseq;          /* h^3   */
        sum4 += hseq * hseq * hseq * hseq;   /* h^4   */
    }

    /* Sum of cubic terms: T(r,c) = (r + c) * sum3 - sum4 */
    seq_t rc = (seq_t)(r + c);
    seq_t T  = rc * sum3 - sum4;
    return T;
}
