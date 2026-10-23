/*
 * A019516  Delete all 2's, 3's, 5's and 7's from the sequence of nonnegative
 *          integers.
 * https://oeis.org/A019516
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn, look
 * Formula:   a(n) = n-th integer after deleting all 2's, 3's, 5's, 7's from
 *            each nonnegative integer in its decimal representation (skip
 *            numbers that become empty)
 * Range:     u8 n <= 347, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t m = 0; ; ++m)
    {
        bool kept = false;
        unsigned long long result = 0;
        unsigned long long pos = 1;

        if (m == 0)
        {
            kept = true;   /* 0 keeps digit 0 */
        }
        else
        {
            idx_t temp = m;
            while (temp > 0)
            {
                unsigned int digit = (unsigned int)(temp % 10);
                temp /= 10;
                if (digit == 2 || digit == 3 || digit == 5 || digit == 7)
                    continue;
                kept = true;
                result += (unsigned long long)digit * pos;
                pos *= 10;
            }
        }

        if (kept)
        {
            if (count == n)
                return (seq_t)result;
            ++count;
        }
    }
}
