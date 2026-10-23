/*
 * A048794  Subsets of natural numbers arranged in standard statistical (or
 *          Yates) order.
 * https://oeis.org/A048794
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, base
 * Formula:   a(n) = concatenation of the positions of 1-bits in the binary
 *            representation of n (positions counted from 1 starting at the
 *            least significant bit)
 * Range:     u8 n <= 14, u16 n <= 62, u32 n <= 1022, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t result = 0;
    idx_t bitpos = 1;
    idx_t tmp = n;

    /* For each set bit, append its position as a decimal block. */
    while (tmp != 0)
    {
        if (tmp & 1UL)
        {
            /* Compute 10^digits(bitpos).  For bitpos <= 64, digits <= 2. */
            seq_t pow10 = 10;
            idx_t t = bitpos;
            if (t >= 10)
            {
                pow10 = 100;
                if (t >= 100)
                {
                    pow10 = 1000;
                    if (t >= 1000)
                    {
                        /* Not reached for pos <= 64, but kept for safety */
                        pow10 = 10000;
                        if (t >= 10000)
                        {
                            pow10 = 100000;
                            if (t >= 100000)
                            {
                                pow10 = 1000000;
                                if (t >= 1000000)
                                {
                                    pow10 = 10000000;
                                    if (t >= 10000000)
                                    {
                                        pow10 = 100000000;
                                        if (t >= 100000000)
                                        {
                                            pow10 = 1000000000;
                                            if (t >= 1000000000)
                                            {
                                                pow10 = 10000000000ull;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }

            result = result * pow10 + bitpos;
        }

        tmp >>= 1;
        bitpos++;
    }

    return result;
}
