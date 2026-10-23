/*
 * A175237  a(n) = 2*(10^n - 7).
 * https://oeis.org/A175237
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*(10^n - 7)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 38,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n < 1)               /* sequence is defined only for n ≥ 1 */
        return 0;

    seq_t val = pow10(n);    /* compute 10^n */
    val -= (seq_t)7;         /* subtract 7 */
    val *= (seq_t)2;         /* multiply by 2 */
    return val;
}
