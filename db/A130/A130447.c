/*
 * A130447  Numbering the days of a 365-day year from 1 (Jan 01) to 365 (Dec
 *          31), these are the days that start months.
 * https://oeis.org/A130447
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   a(n) = month_start[(n-1) mod 12] + 365 * floor((n-1)/12)
 * Range:     u8 n <= 9, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where month_start[0] = 1, month_start[1] = 32, …, month_start[11] = 335  */
seq_t a(idx_t n)
{
    /* n ≥ 1 */
    idx_t month = (n - 1) % 12;
    idx_t year  = (n - 1) / 12;

    /* Days in each month of a non‑leap year */
    const seq_t month_lengths[12] = { (seq_t)31, (seq_t)28, (seq_t)31, (seq_t)30,
                                      (seq_t)31, (seq_t)30, (seq_t)31, (seq_t)31,
                                      (seq_t)30, (seq_t)31, (seq_t)30, (seq_t)31 };

    /* Compute the day of year for the month start */
    seq_t day = (seq_t)1;              /* January 1 is day 1 */
    for (idx_t i = 0; i < month; ++i)
        day += month_lengths[i];

    /* Shift by complete years */
    return day + (seq_t)year * (seq_t)365;
}
