/*
 * A056999  Number of days in months of Persian calendar.
 * https://oeis.org/A056999
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 31 for months 1–6, 30 for months 7–10, 29 or 30 for month
 *            11, where month 11 (Esfand) has 30 days iff the Persian year Y
 *            satisfies (Y * 682) mod 2816 < 682. The Persian year is Y =
 *            floor((n-1)/12) + 1
 * Range:     u8 n <= 11, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* Undefined for n = 0 */

    /* Indexing: months are 0‑based, years start at 1 */
    idx_t year  = (n - 1) / 12 + 1;     /* Persian year */
    idx_t month = (n - 1) % 12;         /* 0‑based month index */

    /* Months 1–6: Farvardin…Shahrivar */
    if (month < 6)
        return 31;

    /* Months 7–10: Mehr…Bahman */
    if (month < 11)
        return 30;

    /* Month 11: Esfand (last month), 29 or 30 days */
    seq_t y      = (seq_t)year;
    seq_t b      = (y * 682) % 2816;    /* 682 * Y mod 2816 */
    bool leap    = b < 682;             /* Leap year condition */

    return leap ? 30 : 29;
}
