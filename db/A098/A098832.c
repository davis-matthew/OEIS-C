/*
 * A098832  Square array read by antidiagonals: even-numbered rows of the
 *          table are of the form n*(n+m) and odd-numbered rows are of the
 *          form n*(n+m)/2.
 * https://oeis.org/A098832
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 136, u16 n <= 1275, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the n‑th term of the antidiagonal reading of the square array
   T defined as follows (rows and columns are 1‑based):

       if (row r is even)  T(r,c) = c * (c + r)
       if (row r is odd)   T(r,c) = c * (c + r) / 2

   The sequence is obtained by traversing the array along anti‑diagonals
   (increasing sum r+c).  Within each diagonal entries are taken with
   increasing row index r (i.e. top to bottom). */

seq_t a(idx_t n)
{
    /* For n≥1 we scan anti‑diagonals until the n‑th element is found. */
    idx_t remaining = n;          /* how many elements still needed */

    for (idx_t diag = 2; ; ++diag)  /* diagonal sum r + c = diag   */
    {
        idx_t count = diag - 1;   /* number of entries on this diagonal */
        if (remaining <= count)
        {
            /* position within this diagonal (0‑based) */
            idx_t offset = remaining - 1;
            idx_t r = 1 + offset;            /* row index  (starting at 1) */
            idx_t c = diag - r;              /* column index */

            /* Compute the array entry. */
            seq_t val = (seq_t)c * ( (seq_t)c + (seq_t)r );
            if (r & 1)            /* odd row → need division by 2           */
                val /= 2;
            return val;
        }
        remaining -= count;          /* skip this diagonal */
    }
}
