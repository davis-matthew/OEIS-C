/*
 * A094798  Number of times 1 is used in writing out all the numbers 1 through
 *          n.
 * https://oeis.org/A094798
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, base, nonn
 * Formula:   a(n) = number of digit 1's appearing in the decimal
 *            representations of all integers from 1 to n. Computed in O(log10
 *            n) time using digit DP
 * Range:     u8 n <= 180, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t N = (seq_t)n;
    seq_t factor = 1;      /* 10^position */
    seq_t result = 0;

    while (factor <= N)
    {
        seq_t next = factor * 10;
        seq_t high = N / next;
        seq_t cur  = (N / factor) % 10;
        seq_t low  = N % factor;

        /* ones contributed by higher digits */
        result += high * factor;

        /* ones contributed by current digit */
        if (cur == 1)
        {
            result += low + 1;
        }
        else if (cur > 1)
        {
            result += factor;
        }

        /* next position */
        factor = next;
    }

    return result;
}
