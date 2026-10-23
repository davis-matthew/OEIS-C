/*
 * A011766  Number of days in A.D. years from 100*n through 100*n + 99.
 * https://oeis.org/A011766
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of days between the years 100*n and 100*n+99
 *            (inclusive) in the calendar used by English-speaking countries.
 *            For each year y: - If y == 0: year does not exist, contributes 0
 *            days. - If y < 1752: Julian leap rule, leap if y % 4 == 0. - If
 *            y >= 1752: Gregorian leap rule, leap if (y %...
 * Range:     u8 none, u16 n <= 32, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the start and end year of the block */
    idx_t start = n * (idx_t)100;
    idx_t end   = start + (idx_t)99;
    seq_t sum = 0;

    for (idx_t y = start; y <= end; ++y) {
        if (y == 0) /* year 0 does not exist */ continue;

        int leap = 0;
        if (y < (idx_t)1752) {
            leap = (y % (idx_t)4 == (idx_t)0);
        } else {
            leap = ((y % (idx_t)400 == (idx_t)0) ||
                    ((y % (idx_t)4 == (idx_t)0) && (y % (idx_t)100 != (idx_t)0)));
        }

        seq_t days = (seq_t)365 + (seq_t)leap;
        if (y == (idx_t)1752) days -= (seq_t)11;  /* skip 11 days in 1752 */
        sum += days;
    }

    return sum;
}
