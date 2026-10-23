/*
 * A031497  Numbers whose base-10 representation has one more 0 than 9's.
 * https://oeis.org/A031497
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: numbers whose base-10 representation has one more 0 than 9's */
#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t found = 0;
    idx_t candidate = 10;                 /* smallest admissible number */

    while (found < n)
    {
        idx_t tmp = candidate;
        unsigned zeros = 0;               /* count of digit 0 */
        unsigned nines = 0;               /* count of digit 9 */

        while (tmp > 0)
        {
            unsigned d = (unsigned)(tmp % 10);
            if (d == 0)
                ++zeros;
            else if (d == 9)
                ++nines;
            tmp /= 10;
        }

        if (zeros == nines + 1)
        {
            ++found;
            if (found == n)
                return (seq_t)candidate;
        }
        ++candidate;
    }

    /* Should never reach here */
    return (seq_t)0;
}
