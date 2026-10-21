/*
 * A090651  Perpetual calendar sequence: There are 14 basic year calendars, 7
 *          for normal years and 7 for leap years. This sequence identifies
 *          the calendars for years 1901 through 2099, when it reinitializes
 *          because 2100 is not a leap year.
 * https://oeis.org/A090651
 *
 * Offset:    1901    (a(1901) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = day‑of‑week value (Sunday = 1, …, Saturday = 7) for the
 *            Gregorian year n, with values 8–14 for leap years. The
 *            calculation is performed by counting the number of leap years up
 *            to n−1 and using the known weekday of 1 Jan 1901 (Tuesday)
 * Range:     u8 n <= 1997, u16 n <= 1997, u32 n <= 1997, u64 n <= 1997,
 *            u128 n <= 1997, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1901)
        return 0;   /* undefined for years before the offset */

    /* Count the number of leap years in the interval [1901, n−1] */
    idx_t start = 1901;
    idx_t end   = n - 1;
    idx_t leaps = 0;
    if (end >= start) {
        idx_t div4   = end / 4      - (start - 1) / 4;
        idx_t div100 = end / 100    - (start - 1) / 100;
        idx_t div400 = end / 400    - (start - 1) / 400;
        leaps = div4 - div100 + div400;
    }

    /* Weekday of 1 Jan 1901 is Tuesday, represented by 2 (0 = Sunday). */
    idx_t shift = ((n - 1901) + leaps) % 7;
    idx_t dow   = (2 + shift) % 7;          /* 0–6, 0 = Sunday */

    seq_t val = dow + 1;    /* 1–7 for normal years */

    /* Add 7 if year n is a leap year */
    if ((n % 4 == 0) && ((n % 100 != 0) || (n % 400 == 0)))
        val += 7;          /* 8–14 for leap years */

    return val;
}
