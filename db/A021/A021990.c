/*
 * A021990  Decimal expansion of 1/986.
 * https://oeis.org/A021990
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor(10^(n+1) / 986) mod 10
 * Range:     u8 n <= 3, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t rem   = (seq_t)1;   /* current remainder in long division */
    seq_t digit = 0;         /* digit at position n */

    for (idx_t i = 0; i <= n; ++i) {
        seq_t prod   = rem * (seq_t)10;
        digit        = prod / (seq_t)986;
        rem          = prod % (seq_t)986;
    }
    return digit;
}
