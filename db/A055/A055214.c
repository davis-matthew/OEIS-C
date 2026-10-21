/*
 * A055214  a(0) = 1; a(n) = 2*n*a(n-1) - 1 for n >= 1.
 * https://oeis.org/A055214
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1; 
   a(n) = 2*n*a(n-1) - 1  for n >= 1   */
#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t res = 1;                 /* a(0) */
    for (idx_t i = 1; i <= n; ++i) {
        res = (seq_t)2 * (seq_t)i * res - 1;
    }
    return res;
}
