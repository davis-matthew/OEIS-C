/*
 * A227181  Irregular table: integers from n to n^2 followed by integers from
 *          (n+1) to (n+1)^2.
 * https://oeis.org/A227181
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = A227177(n) + A227179(n) * * The sequence is produced by
 *            an irregular table. Row r (starting at 0) * contains the
 *            integers from r to r² inclusive, i.e. r² − r + 1 entries. * The
 *            flattened sequence reads the rows consecutively. * To find a(n):
 *            * *Find the row r such that the running total of...
 * Range:     u8 n <= 1375, u16 n <= 9951, u32 n <= 9951, u64 n <= 9951,
 *            u128 n <= 9951, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* running cumulative count of entries processed so far */
    idx_t cumulative = 0;
    /* current row index */
    idx_t r = 0;
    /* number of entries in the current row: r² − r + 1
     * start with r = 0 → 1 entry (the integer 0). */
    idx_t row_len = 1;

    while (n >= cumulative + row_len) {
        cumulative += row_len;   /* skip the entire row */
        ++r;                      /* move to next row */
        /* row_len increases by 2*(r-1) when r increments */
        row_len += 2 * (r - 1);
    }

    /* position inside the current row */
    idx_t offset = n - cumulative;
    /* value is r + offset (both are unsigned integers) */
    return (seq_t)r + (seq_t)offset;
}
