/*
 * A032561  Numbers k whose decimal representation, read as a base-15 value
 *          and divided by k, yields an integer.
 * https://oeis.org/A032561
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n-th integer k whose decimal representation,
 *            interpreted as a base‑15 number, is divisible by k. The
 *            implementation simply scans integers in increasing order and
 *            tests the divisibility condition
 * Range:     u8 n <= 11, u16 n <= 17, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t candidate = 1;

    while (1) {
        /* compute the base‑15 value of the decimal digits of candidate */
        unsigned __int128 val   = 0;
        unsigned __int128 pow15 = 1;
        idx_t tmp = candidate;

        while (tmp > 0) {
            int d = tmp % 10;           /* current decimal digit */
            val += (unsigned __int128)d * pow15;
            pow15 *= 15;
            tmp /= 10;
        }

        /* check divisibility */
        if (val % candidate == 0) {
            ++count;
            if (count == n)
                return (seq_t)candidate;
        }
        ++candidate;
    }
}
