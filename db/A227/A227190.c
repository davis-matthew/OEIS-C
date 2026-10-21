/*
 * A227190  a(n) = n minus (product of run lengths in binary representation of
 *          n).
 * https://oeis.org/A227190
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n minus (product of run lengths in binary representation
 *            of n)
 * Range:     u8 n <= 260, u16 n <= 4096, u32 n <= 4096, u64 n <= 4096,
 *            u128 n <= 4096, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* n should be >= 1, but guard for 0 */

    /* Find most significant bit index of n (0-based). */
    int msb = 63 - __builtin_clzll(n);      /* n is uint64_t, so safe */

    seq_t prod = 1;                        /* product of run lengths */
    int last_bit = (n >> msb) & 1;          /* bit at most significant position */
    int run = 1;                           /* current run length */

    /* Iterate over remaining bits from msb-1 down to 0. */
    for (int i = msb - 1; i >= 0; --i)
    {
        int bit = (n >> i) & 1;
        if (bit == last_bit)
        {
            ++run;
        }
        else
        {
            prod *= (seq_t)run;
            run = 1;
            last_bit = bit;
        }
    }

    /* Final run contributes to the product. */
    prod *= (seq_t)run;

    /* Result is n minus the product of run lengths. */
    return (seq_t)n - prod;
}
