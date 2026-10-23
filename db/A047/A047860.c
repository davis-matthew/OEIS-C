/*
 * A047860  a(n) = T(3,n), array T given by A047858.
 * https://oeis.org/A047860
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1 a(n) = 2^(n-1) * (n + 6) - 2 for n >= 1
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 27, u64 n <= 58, u128 n <= 122,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t p   = pow2(n - 1);          /* 2^(n-1) */
    seq_t m   = (seq_t)n + 6;         /* n + 6    */
    seq_t prod = p * m;               /* product   */
    seq_t res  = prod - 2;            /* minus 2   */
    return res;
}
