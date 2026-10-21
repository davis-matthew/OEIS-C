/*
 * A215414  Timestamp for start of year, beginning with 1970, similar to Unix.
 * https://oeis.org/A215414
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, less
 * Formula:   a(n) = 86400 * ( 365 * (n - 1) + ((n + 1968) / 4 - 492) ) (Naïve
 *            leap‑year counting: every year divisible by 4 is leap.)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 137, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to number of days since 1970‑01‑01.
       n = 1  → year 1970 (0 days)
       n = 2  → year 1971 (1 year)
       ...
    */

    /* Base days: each normal year contributes 365 days. */
    seq_t days = (seq_t)(n - 1) * 365;

    /* Additional leap days: every 4th year since 1970. */
    days += (seq_t)((n + 1968) / 4 - 492);

    /* Convert days to seconds. */
    return days * 86400;
}
