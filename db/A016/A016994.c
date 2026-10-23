/*
 * A016994  a(n) = (7*n + 1)^2.
 * https://oeis.org/A016994
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*n + 1)^2
 * Range:     u8 n <= 2, u16 n <= 36, u32 n <= 9362, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;   /* cast index to seq_t first */
    x *= 7;               /* 7*n  */
    x += 1;               /* 7*n + 1 */
    return x * x;         /* (7*n + 1)^2 */
}
