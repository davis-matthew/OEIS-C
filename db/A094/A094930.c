/*
 * A094930  Triangle T(n,m) read by rows, defined by squaring a matrix with
 *          row entries 2+3*(m-1).
 * https://oeis.org/A094930
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 576, u32 n <= 9717, u64 n <= 9717,
 *            u128 n <= 9717, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A094930: Triangle T(n,m) read by rows.  The formula used is
   T(r,m) = (3*m-1)*(3*m+3*r-2)*(r+1-m)/2
   where r >= 1 is the row number and 1 <= m <= r is the column number.
   The sequence a(n) is obtained by listing T(r,m) row by row:
   a(1)=T(1,1), a(2)=T(2,1), a(3)=T(2,2), a(4)=T(3,1), …   */

/* Return the nth term of A094930, with the offset being 1. */
seq_t a(idx_t n)
{
    /* Handle n < 1 – not used by the library, but guard anyway. */
    if (n < 1) return 0;

    /* Find the row r such that r*(r+1)/2 >= n.
       Since n is typically modest (up to ~10⁵), a simple linear search
       is fast enough and keeps the implementation simple and
       independent of any non‑standard math routines. */
    idx_t r = 1;
    /* r*(r+1)/2 grows roughly like r²/2, so this loop iterates O(sqrt(n)) times. */
    while (((r * (r + 1)) / 2) < n) {
        ++r;
    }

    /* Column index m = n - number of terms in previous rows. */
    idx_t prev_terms = (r - 1) * r / 2;   /* sum_{k=1}^{r-1} k */
    idx_t m = n - prev_terms;             /* 1 <= m <= r */

    /* Compute T(r,m) using the closed formula, cast everything to seq_t
       to avoid overflow in intermediate 64‑bit products. */
    seq_t term1 = (seq_t)(3 * m - 1);
    seq_t term2 = (seq_t)(3 * m + 3 * r - 2);
    seq_t term3 = (seq_t)(r + 1 - m);

    /* Multiply and divide by 2.  The division is exact for all
       integer arguments, so no rounding issues. */
    seq_t result = term1 * term2 * term3 / 2;

    return result;
}
