/*
 * A218036  a(n) = (n+1) + (n+3/2)*H(n) - (H(n)^3)/2, where H(n) = A002024(n).
 * https://oeis.org/A218036
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (k + 1) * (r + 1) where * r = smallest integer with
 *            r*(r+1)/2 >= n (row number, 1‑based) * k = n - r*(r-1)/2 (column
 *            position, 1‑based)
 * Range:     u8 n <= 119, u16 n <= 3655, u32 n <= 3655, u64 n <= 3655,
 *            u128 n <= 3655, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset 1; n=0 not part of sequence */

    seq_t N = (seq_t)n;                 /* convert index to seq_t type */

    seq_t r   = 1;
    seq_t cum = 1;                      /* triangular number T(r) = r*(r+1)/2 */

    /* Find the row r such that T(r-1) < n <= T(r) */
    while (cum < N) {
        r++;
        cum += r;
    }

    seq_t prev = cum - r;               /* T(r-1) */
    seq_t k    = N - prev;              /* column index (1‑based) */

    return (k + 1) * (r + 1);           /* a(n) = (k+1)*(r+1) */
}
