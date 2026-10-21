/*
 * A236990  Number of (n+1)X(1+1) 0..2 arrays with the maximum plus the lower
 *          median minus the upper median minus the minimum of every 2X2
 *          subblock differing from its horizontal and vertical neighbors by
 *          exactly one.
 * https://oeis.org/A236990
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 400 * a(n-4), a(1)=81, a(2)=360, a(3)=1600, a(4)=7200
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 12, u64 n <= 27, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Modulo 4 base values */
    static const seq_t base_values[4] = {7200, 81, 360, 1600};
    /* All calculations are on seq_t, so use seq_t constant for 400 */
    const seq_t factor = (seq_t)400;

    /* n >= 1 */
    idx_t mod = n % 4;
    idx_t k   = (n - 1) / 4;          /* number of times to multiply by factor */

    seq_t result = base_values[mod];
    for (idx_t i = 0; i < k; ++i)
        result *= factor;            /* works for all seq_t types */
    return result;
}
