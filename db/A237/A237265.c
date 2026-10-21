/*
 * A237265  Irregular table: n X n matrices (n=1,2,3,...), read by rows filled
 *          with numbers 1..n, with k moved to the front in the k-th row.
 * https://oeis.org/A237265
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute OEIS sequence A237265.
   The sequence is obtained by reading, row by row, the infinite
   irregular table of n×n matrices.  
   Matrix n is built by taking the numbers 1..n and moving row k to the
   front in row k (rows are 1‑based).  The table is then flattened
   by concatenating the rows of successive matrices, starting with
   the 1×1 matrix.  a(n) is the n-th element of that flattened list.  */
   
seq_t a(idx_t n)
{
    /* ----------------------------------------------------------------------
     * Find the matrix size k such that the first n entries are included in
     * the first k×k matrix.  The cumulative number of entries up to size k
     * is sum_{i=1}^k i^2 = k(k+1)(2k+1)/6, but we compute it incrementally
     * to avoid overflow.
     * ---------------------------------------------------------------------- */
    idx_t k = 1;                     /* current matrix size (>=1)          */
    idx_t cumulative = 1;            /* sum i^2 for i = 1 .. k            */
    while (cumulative < n)
    {
        ++k;
        cumulative += k * k;
    }

    /* ----------------------------------------------------------------------
     * Position of n inside the k×k matrix (0‑based).
     * previous_sum = total entries for matrices 1..k-1
     * offset        = index of n inside current matrix.
     * ---------------------------------------------------------------------- */
    idx_t previous_sum = cumulative - k * k;
    idx_t offset = n - previous_sum - 1;        /* 0‑based index within matrix */

    /* ----------------------------------------------------------------------
     * Translate offset to 1‑based row and column within matrix.
     * Rows are numbered 1..k from top to bottom.
     * Columns are numbered 1..k from left to right.
     * ---------------------------------------------------------------------- */
    idx_t row = offset / k + 1;              /* 1‑based row number */
    idx_t col = offset % k + 1;              /* 1‑based column number */

    /* ----------------------------------------------------------------------
     * Compute the value at (row, col) in the k×k matrix.
     * Row r is created by moving r to the front; the remaining numbers
     * 1..k \{r} stay in increasing order.
     *
     * If col == 1 then the value is r.
     * Otherwise let c' = col-1 (index into the remaining sequence).
     * The c'-th element of {1..k} \ {r} is:
     *      c'  if c' < r
     *  c'+1  otherwise
     * ---------------------------------------------------------------------- */
    seq_t value;
    if (col == 1)
    {
        value = (seq_t)row;               /* first element of the row */
    }
    else
    {
        idx_t cprime = col - 1;           /* 1‑based index inside the
                                             remaining sequence */
        if (cprime < row)
            value = (seq_t)cprime;
        else
            value = (seq_t)(cprime + 1);
    }
    return value;
}
