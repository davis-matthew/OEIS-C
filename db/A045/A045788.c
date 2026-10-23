/*
 * A045788  Squares with initial digit '5'.
 * https://oeis.org/A045788
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the square of the n-th integer whose square starts with
 *            digit 5 (search-based generation)
 * Range:     u8 none, u16 n <= 30, u32 n <= 3118, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n < 1, return 0 */
    idx_t found = 0;
    idx_t m = 1;

    while (1) {
        seq_t sq = (seq_t)m * m;        /* compute square in seq_t */
        seq_t ld = sq;                  /* extract leading digit */
        while (ld >= 10) ld /= 10;

        if (ld == (seq_t)5) {           /* square starts with 5? */
            ++found;
            if (found == n)
                return sq;             /* nth term found */
        }
        ++m;
    }
}
