/*
 * A140976  Period length 10: repeat 8, 8, 6, 6, 4, 4, 2, 2, 0, 0.
 * https://oeis.org/A140976
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Periodic sequence with period 10: a(n) = {8,8,6,6,4,4,2,2,0,0}[n mod 10] */
seq_t a(idx_t n)
{
    static const seq_t cycle[10] = {8, 8, 6, 6, 4, 4, 2, 2, 0, 0};
    return cycle[(size_t)(n % 10)];
}
