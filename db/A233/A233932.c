/*
 * A233932  Irregular table read by rows: T(n,k) is the binary representation
 *          of n shifted right k times and incremented if the last bit shifted
 *          away was set.
 * https://oeis.org/A233932
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is obtained by reading an irregular table row‑by‑row. The
 *            table is defined by T(n,k) = (n >> k) + ((n >> (k-1)) & 1) (1 ≤
 *            k ≤ ⌊log₂(n)⌋ + 1) The sequence a(n) is the n‑th element of the
 *            concatenation of all rows of this table
 * Range:     u8 n <= 106, u16 n <= 106, u32 n <= 106, u64 n <= 106,
 *            u128 n <= 106, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* No external headers required other than the type definitions of
   seq_t and idx_t that are supplied by the build system. */

static unsigned int floor_log2_idx(idx_t v)
{
    unsigned int r = 0;
    while (v >>= 1u)  /* shift right until zero; count shifts */
        ++r;
    return r;
}

seq_t a(idx_t n)
{
    /* Offset is 1; for n < 1 the value is undefined – return 0. */
    if (n == 0)
        return 0;

    idx_t cum = 0;          /* cumulative number of terms processed */
    idx_t row = 0;           /* current row number */
    unsigned int len = 0;   /* number of terms in the current row */

    /* Find the row r such that prev_cum < n <= cum. */
    while (cum < n) {
        ++row;
        len = floor_log2_idx(row) + 1;   /* floor(log₂(row)) + 1 */
        cum += len;
    }

    idx_t prevCum = cum - len; /* terms before this row */
    idx_t pos = n - prevCum;   /* position within the row (1‑based) */

    /* Compute T(row, pos). */
    idx_t base = row;
    idx_t k = pos;             /* same as the position in the row */

    idx_t shiftRight = base >> k;
    idx_t inc = (base >> (k - 1)) & 1u;

    return (seq_t)(shiftRight + inc);
}
