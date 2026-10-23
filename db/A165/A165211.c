/*
 * A165211  Period 8: repeat [0,1,0,1,1,0,1,0].
 * https://oeis.org/A165211
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Period 8: repeat [0,1,0,1,1,0,1,0] (a(0)=0, a(1)=1, …)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Sequence is purely periodic; no need for large computations. */
    static const seq_t pat[8] = {0, 1, 0, 1, 1, 0, 1, 0};
    return pat[(size_t)(n & 7u)];
}
