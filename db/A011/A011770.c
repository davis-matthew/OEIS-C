/*
 * A011770  Days per century for British calendar from first century,
 *          following Gregorian calendar after A.D. 1752.
 * https://oeis.org/A011770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of days in the n‑th British century. • Years <
 *            1752 use the Julian calendar (leap years every 4th year). •
 *            Years > 1752 use the Gregorian calendar (leap years every 4th
 *            year, except years divisible by 100 unless also divisible by
 *            400). • Year 1752 had 355 days (366 – 11) because 11...
 * Range:     u8 none, u16 n <= 33, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                       /* undefined for n < 1 */

    idx_t start = 100 * (n - 1) + 1;             /* first year of the century */
    idx_t end   = 100 * n;                       /* last year of the century */
    seq_t days  = 0;

    for (idx_t y = start; y <= end; ++y) {
        if (y == 1752) {
            days += (seq_t)355;                  /* 366 minus 11 skipped days */
        } else if (y < 1752) {
            days += (y % 4 == 0) ? (seq_t)366 : (seq_t)365;
        } else { /* y > 1752, Gregorian rules */
            idx_t m4   = y % 4;
            idx_t m100 = y % 100;
            idx_t m400 = y % 400;
            days += (m4 == 0 && (m100 != 0 || m400 == 0))
                    ? (seq_t)366
                    : (seq_t)365;
        }
    }

    return days;
}
