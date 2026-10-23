/*
 * A182688  a(n) = the largest 1-digit number with exactly n divisors, a(n) =
 *          0 if no such number exists.
 * https://oeis.org/A182688
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the largest one‑digit number that has exactly n divisors.
   Returns 0 if no such digit exists. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* target divisor count */
    idx_t target = n;

    /* scan digits from 9 down to 1 */
    for (int d = 9; d >= 1; --d)
    {
        idx_t cnt = 0;
        /* count divisors of d */
        for (int i = 1; i <= d; ++i)
            if (d % i == 0)
                ++cnt;

        if (cnt == target)
            return (seq_t)d;
    }
    return (seq_t)0;
}
