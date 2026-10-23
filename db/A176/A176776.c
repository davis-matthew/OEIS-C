/*
 * A176776  a(n) = 2^n - n*(n-2).
 * https://oeis.org/A176776
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n - n*(n-2)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t result = pow2(n);
    if (n >= 2) {
        seq_t nval = (seq_t)n;
        result -= nval * (nval - 2);
    } else if (n == 1) {
        result += (seq_t)1;   /* subtracting (-1) is equivalent to adding 1 */
    }   /* for n == 0 nothing needs to be changed */
    return result;
}
