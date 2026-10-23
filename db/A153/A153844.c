/*
 * A153844  First Sunday in n-th month of 365-day year starting on Friday.
 * https://oeis.org/A153844
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((7 - ((5 + ((n-1)/12) + S[(n-1)%12]) % 7)) % 7) + 1
 *            where S = {0,3,3,6,1,4,6,2,5,0,3,5,1} gives cumulative days mod
 *            7 before each month in a 12‑month year
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* outside the sequence's offset */

    /* Day of week numbers: 0=Sunday, 1=Monday, …, 6=Saturday.
       The year 1 starts on Friday = 5.  Each year shifts start by +1 day. */
    idx_t year_offset = (n - 1) / 12;          /* number of whole years before month n */
    int  month_idx = (int)((n - 1) % 12);      /* 0-based month index within the year */

    /* Cumulative days mod 7 before each month in a standard 12‑month year */
    static const int prefix_mod[13] = {
        0, 3, 3, 6, 1, 4, 6, 2, 5, 0, 3, 5, 1
    };

    int weekday_year_start = (5 + (int)(year_offset % 7)) % 7;
    int offset_days = prefix_mod[month_idx];
    int month_start_weekday = (weekday_year_start + offset_days) % 7;

    int first_sunday = ((7 - month_start_weekday) % 7) + 1;
    return (seq_t)first_sunday;
}
