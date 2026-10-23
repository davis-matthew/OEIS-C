/*
 * A153840  First Sunday in the n-th month of a 365-day year starting on
 *          Monday.
 * https://oeis.org/A153840
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = day of month of the first Sunday in month n of a 365‑day
 *            year that starts on Monday
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* lengths of the 12 months in a non‑leap year */
    static const idx_t month_len[12] = {31,28,31,30,31,30,31,31,30,31,30,31};

    /* month index 0..11 (Jan=0), period 12 */
    idx_t m = (n - 1) % 12;

    /* cumulative days before month m */
    idx_t cum = 0;
    for (idx_t i = 0; i < m; ++i)
        cum += month_len[i];

    /* weekday of the first day of month m:
       1 = Monday … 7 = Sunday */
    idx_t dow = (cum % 7) + 1;

    /* days to add to reach Sunday */
    idx_t offset = (7 - dow) % 7;

    /* return day of month (1..31) of the first Sunday */
    return (seq_t)(offset + 1);
}
