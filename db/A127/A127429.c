/*
 * A127429  2^(n*(n-1)/2) - n!.
 * https://oeis.org/A127429
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(n*(n-1)/2) - n!
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   factorial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    if (n <= 2)
        return 0;

    /* exponent = n*(n-1)/2 */
    idx_t exp = (n * (n - 1)) / 2;

    seq_t pow = pow2(exp);        /* 2^exp */
    seq_t fact = factorial(n);    /* n! */

    return pow - fact;
}
