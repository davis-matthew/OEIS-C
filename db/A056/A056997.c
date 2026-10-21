/*
 * A056997  Number of days in months of Ethiopic calendar.
 * https://oeis.org/A056997
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 30 if n % 13 != 0 else a(n) = 5 + ((n / 13) % 4 == 0 ? 1
 *            : 0) (i.e., intercalary month has 5 days normally, 6 days every
 *            4th year)
 * Range:     u8 n <= 52, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)               /* undefined for offset 1, but return 0 */
        return 0;

    /* months 1..12 of each year: 30 days */
    if (n % 13 != 0)
        return 30;

    /* intercalary month – month 13 of each year */
    idx_t year = n / 13;      /* year number (1‑based) */
    if (year % 4 == 0)
        return 6;             /* leap year */
    return 5;                 /* common year */
}
