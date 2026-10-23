/*
 * A144189  Table in which row n lists the digits of n, the digits of the
 *          number of days in each month of year n and the digits of the
 *          number of days in year n.
 * https://oeis.org/A144189
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, base, less
 * Formula:   a(n) is the nth digit in the concatenation of the decimal digits
 *            of the year number, the decimal digits of the days in each month
 *            for that year, and the decimal digits of the total number of
 *            days in that year
 * Range:     u8 n <= 106, u16 n <= 106, u32 n <= 106, u64 n <= 106,
 *            u128 n <= 106, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* days in each month: index 0..11 (January..December) */
    const int monthBase[12] = {31, 28, 31, 30, 31, 30,
                               31, 31, 30, 31, 30, 31};

    idx_t remaining = n;                /* remaining index into sequence */

    for (idx_t year = 1; ; ++year) {
        /* ----- compute digits of the year number ----- */
        idx_t tmp = year;
        int  rev[20];                   /* store digits in reverse order   */
        int  lenYear = 0;
        while (tmp > 0) {
            rev[lenYear++] = tmp % 10;
            tmp /= 10;
        }

        idx_t rowLen = (idx_t)lenYear + 27;   /* year digits + 24 (M) + 3 (total) */
        if (remaining <= rowLen) {
            idx_t offset = remaining - 1;     /* 0‑based offset in this row */

            /* ---- digits of the year number ---- */
            if (offset < (idx_t)lenYear) {
                int d = rev[lenYear - 1 - offset];
                return (seq_t)d;
            }
            offset -= lenYear;

            /* ---- days per month ---- */
            bool isLeap = ( (year % 400 == 0) ||
                            ((year % 4 == 0) && (year % 100 != 0)) );

            if (offset < 24) {               /* 12 months × 2 digits each */
                int monthIdx   = offset / 2;
                int digitIndex = offset % 2;

                int day = (monthIdx == 1)
                          ? (isLeap ? 29 : 28)
                          : monthBase[monthIdx];

                int d = (digitIndex == 0) ? day / 10 : day % 10;
                return (seq_t)d;
            }
            offset -= 24;

            /* ---- total days in the year ---- */
            int total = isLeap ? 366 : 365;
            int tdigs[3];
            tdigs[0] = total / 100;
            tdigs[1] = (total / 10) % 10;
            tdigs[2] = total % 10;

            return (seq_t)tdigs[offset];     /* offset is 0,1,2 */
        }

        remaining -= rowLen;                 /* skip this whole row */
    }
}
