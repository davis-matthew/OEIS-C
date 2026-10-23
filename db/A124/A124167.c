/*
 * A124167  a(n) = 10*(10^n-1).
 * https://oeis.org/A124167
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 10*(10^n-1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    seq_t p = pow10(n);     /* compute 10^n  */
    p -= (seq_t)1;          /* subtract 1   */
    p *= (seq_t)10;         /* multiply by 10 */
    return p;
}
