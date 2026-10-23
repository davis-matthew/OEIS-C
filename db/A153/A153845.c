/*
 * A153845  First Sunday in n-th month of 365-day year starting on Saturday.
 * https://oeis.org/A153845
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   day number of the first Sunday in the n-th month of a 365‑day
 *            year that begins on Saturday
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const idx_t monthLen[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
    idx_t year = (n - 1) / 12;     /* number of full years preceding month n */
    idx_t m     = (n - 1) % 12;    /* month index within the year (0‑based) */

    /* absolute day number of the first day of month n */
    idx_t absStart = 1 + 365 * year;
    for (idx_t i = 0; i < m; ++i)
        absStart += monthLen[i];

    /* weekday of that first day: 0 = Sunday, 1 = Monday, …, 6 = Saturday
       Saturday is the day 1 of the year, so w = (6 + absStart‑1) mod 7. */
    idx_t w = (6 + (absStart - 1)) % 7;

    /* days to add to reach the first Sunday within the month */
    idx_t offset = (7 - w) % 7;

    return (seq_t)(1 + offset);   /* day of month */
}
