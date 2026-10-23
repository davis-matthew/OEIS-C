/*
 * A127511  a(n) = mu(n) * 2^(n-1).
 * https://oeis.org/A127511
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = mu(n) * 2^(n-1)
 * Range:     i8 n <= 9, i16 n <= 16, i32 n <= 32, i64 n <= 41, i128 n <= 41,
 *            big any n
 * Helpers:   mobius, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mobius.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* offset 1, but return 0 for safety */
    seq_t mu = mobius(n);
    seq_t p  = pow2(n - 1);
    return mu * p;
}
