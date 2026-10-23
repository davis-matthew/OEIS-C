/*
 * A045862  Numbers whose square has initial digit '8'.
 * https://oeis.org/A045862
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 2566, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A045862: numbers whose square has initial digit '8' (OEIS) */
#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t count     = 0;
    seq_t candidate = 1;

    while (1)
    {
        /* Square of the candidate */
        seq_t sq = candidate * candidate;

        /* Extract the most significant decimal digit of sq */
        seq_t d = sq;
        while (d >= 10)
            d /= 10;

        if (d == 8)
        {
            ++count;
            if ((idx_t)count == n)
                return candidate;
        }
        ++candidate;
    }
}
