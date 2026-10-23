/*
 * A008686  Day-of-month at weekly intervals from Jan 01 of a non-leap year (a
 *          finite sequence with 53 terms).
 * https://oeis.org/A008686
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = day of month of the date that is 1+(n-1)*7 days from Jan
 *            1 in a non‑leap year
 * Range:     u8 n <= 37, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* day of year (1‑based) */
    seq_t d = ((seq_t)(n - 1) * 7) + 1;

    /* lengths of the months in a non‑leap year */
    static const unsigned char month_len[12] =
    { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    /* find the month containing day 'd' */
    int m = 0;
    while (m < 12 && d > month_len[m]) {
        d -= month_len[m];
        ++m;
    }
    return d;   /* day of month */
}
