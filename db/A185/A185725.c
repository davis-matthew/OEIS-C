/*
 * A185725  Array associated with squares, by antidiagonals.
 * https://oeis.org/A185725
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 136, u16 n <= 1275, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is obtained by traversing the array T(i,j) along its antidiagonals.
   For each n≥1 find the smallest integer d such that d*(d-1)/2 ≥ n.
   Let pos = n − (d−1)(d−2)/2 − 1   (0‑based index inside the d‑th diagonal).
   Then i = pos + 1,       j = d − i.
   Finally:
        if (i ≤ j)  a(n) = (j‑1)² + 2*i ‑ 1
        else        a(n) = (i‑1)² + 2*j       */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined below the offset */

    idx_t d = 2;
    /* Increase d until the number of terms up to that diagonal reaches n. */
    while ((unsigned __int128)d * (d - 1) / 2 < n) d++;

    idx_t prev = (d - 1) * (d - 2) / 2;
    idx_t pos  = n - prev - 1;          /* 0‑based position in this diagonal */

    idx_t i = pos + 1;
    idx_t j = d - i;

    /* Compute the value using the given piecewise formula.  All arithmetic
       is performed in seq_t to avoid overflow in the 128‑bit build. */
    if (i <= j) {
        seq_t jj = (seq_t)(j - 1);
        return jj * jj + (seq_t)(2 * i - 1);
    } else {
        seq_t ii = (seq_t)(i - 1);
        return ii * ii + (seq_t)(2 * j);
    }
}
