/*
 * A101312  Number of "Friday the 13ths" in year n (starting at 1901).
 * https://oeis.org/A101312
 *
 * Offset:    1901    (a(1901) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = #{m∈{1..12} | the 13th of month m in year n is a Friday}
 * Range:     u8 n <= 2005, u16 n <= 2005, u32 n <= 2005, u64 n <= 2005,
 *            u128 n <= 2005, big any n
 * Helpers:   day_of_week
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The day of week is computed via the Doomsday/Zeller congruence. */
#include <stdint.h>
#include "../utils/day_of_week.h"

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
