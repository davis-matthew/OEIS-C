/*
 * A152756  Bisection of A000533.
 * https://oeis.org/A152756
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, less
 * Formula:   a(1)=1; for n>1, a(n)=10^(2n-2)+1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 12,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n == 1) {
        return (seq_t)1;
    }
    /* Compute exponent 2n-2 as 2*(n-1) to avoid overflow for n>1 */
    idx_t exp = 2 * (n - 1);
    seq_t val = pow10(exp);   /* 10^(2n-2) */
    return val + (seq_t)1;    /* +1 */
}
