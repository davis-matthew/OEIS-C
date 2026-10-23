/*
 * A110568  Period 6: repeat [1, 0, 2, 2, 0, 1].
 * https://oeis.org/A110568
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = {1,0,2,2,0,1}[n mod 6] – periodic with period 6
 * Range:     u8 n <= 92, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t pattern[6] = {1, 0, 2, 2, 0, 1};
    return pattern[(size_t)(n % 6)];
}
