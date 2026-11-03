/*
 * A101312  Number of "Friday the 13ths" in year n (starting at 1901).
 * https://oeis.org/A101312
 *
 * Offset:    1901    (a(1901) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = #{m∈{1..12} | the 13th of month m in year n is a Friday}
 * Range:     u8 n <= 3000, u16 n <= 3000, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Validated: standard, pass, 121 terms through n = 2936 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The day of week is computed via the Doomsday/Zeller congruence. */
#include <stdint.h>

static int day_of_week(idx_t year, idx_t month, idx_t day)
{
    /* Parameters: year ≥ 0, month 1..12, day ≥ 1.
       Return 0=Sunday, 1=Monday, …, 6=Saturday. */
    idx_t a = (14 - month) / 12;          /* 0 if month ≥ 3, 1 if month ≤ 2 */
    idx_t y = year - a;
    idx_t m = month + 12 * a - 2;          /* March=1, …, February=12 */

    unsigned long long sum = day;
    sum += y;
    sum += y / 4;
    sum -= y / 100;
    sum += y / 400;
    sum += (31 * m) / 12;

    return (int)(sum % 7);
}

seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t month = 1; month <= 12; ++month)
    {
        if (day_of_week(n, month, 13) == 5)   /* 5 = Friday */
            count += 1;
    }
    return count;
}
