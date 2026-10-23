/*
 * A116369  Day of the week corresponding to Jan 01 of a given year (n=0 for
 *          the year 2000).
 * https://oeis.org/A116369
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((6 + n + L(n)) mod 7) + 1, where L(n) is the number of
 *            leap years in the inclusive range [2000, 2000+n-1]
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Precompute the leap-year count up to 1999 */
    idx_t start_leaps = (idx_t)1999/4 - (idx_t)1999/100 + (idx_t)1999/400;

    /* Last year in the range [2000, 2000+n-1] */
    idx_t last = (idx_t)2000 + n - 1;

    /* Leap-year count up to 'last' */
    idx_t last_leaps = last/4 - last/100 + last/400;

    /* Number of leap years in the desired range */
    idx_t leaps = last_leaps - start_leaps;

    /* Compute the day of week (0 = Sunday ... 6 = Saturday) */
    idx_t n_mod7     = n % 7;
    idx_t leaps_mod7 = leaps % 7;
    idx_t day_num    = ((idx_t)6 + n_mod7 + leaps_mod7) % 7;

    /* Convert to 1 = Sunday ... 7 = Saturday */
    return (seq_t)day_num + 1;
}
