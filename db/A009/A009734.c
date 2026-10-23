/*
 * A009734  Days in a one-month gym membership starting at n-th day of year [
 *          days from Jan 01 to Feb 01, Jan 02 to Feb 02, ... ].
 * https://oeis.org/A009734
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Let m(n) be the month of the n-th day of a non‑leap year (0=Jan,
 *            ...,11=Dec), and d(n) the day number within that month
 *            (1‑based). Let L[m] be the length of month m
 *            (31,28,31,30,31,30,31,31,30,31,30,31). Then a(n) = L[m] if d(n)
 *            ≤ L[(m+1) mod 12] L[m] - d(n) + 1 + L[(m+1) mod 12] otherwise
 * Range:     u8 n <= 48, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* month lengths in a non‑leap year */
    static const int month_len[12] = {
        31, 28, 31, 30, 31, 30,
        31, 31, 30, 31, 30, 31
    };

    /* Day index within a 365‑day year (0‑based) */
    idx_t day0 = (n - 1) % 365;

    /* Determine month (0‑based) and day of month (1‑based) */
    int month = 0;
    int cum   = 0;
    while (month < 12) {
        if (day0 < (idx_t)(cum + month_len[month])) {
            break;
        }
        cum += month_len[month];
        month++;
    }
    int day_in_month = (int)(day0 - cum) + 1;  /* 1‑based */

    int cur_len = month_len[month];
    int next_len = month_len[(month + 1) % 12];

    int result;
    if (day_in_month <= next_len) {
        result = cur_len;
    } else {
        result = cur_len - day_in_month + 1 + next_len;
    }

    return (seq_t)result;
}
