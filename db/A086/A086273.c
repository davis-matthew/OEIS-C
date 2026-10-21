/*
 * A086273  Rectangular array T(n,k) of central polygonal numbers, by
 *          antidiagonals.
 * https://oeis.org/A086273
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (r+1) * C(c,2) + 1 where (r,c) are obtained from n by
 *            traversing the transpose of the array T(n,k) = (k+1)*C(n,2)+1
 *            along antidiagonals with rows decreasing from the right endpoint
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Return the nth term (1‑based) of OEIS A086273 */
seq_t a(idx_t n)
{
    /* Find the diagonal s such that (s‑1)*s/2 ≥ n.
       Each diagonal s (sum r+c) has length s‑1. */
    uint64_t s = 2;
    while ((s - 1) * s / 2 < n) {
        s++;
    }

    /* Number of terms before this diagonal */
    uint64_t prev = (s - 2) * (s - 1) / 2;

    /* Position within the diagonal (0‑based) */
    uint64_t pos = n - prev - 1;

    /* Coordinates in the transposed array (1‑based) */
    uint64_t r = s - 1 - pos;  /* row index in transpose */
    uint64_t c = 1 + pos;      /* column index in transpose */

    /* Compute (r+1) * C(c,2) + 1 */
    seq_t bin = (seq_t)c * (seq_t)(c - 1) / 2;   /* binomial(c,2) */
    seq_t val = ((seq_t)(r + 1) * bin) + 1;      /* final value */

    return val;
}
