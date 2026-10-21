/*
 * A096551  Consecutive internal states of a linear congruential pseudo-random
 *          number generator with a parameter proposed by George Marsaglia as
 *          a "candidate for the best of all multipliers".
 * https://oeis.org/A096551
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 69069^(n-1) mod 2^32 (a(1)=1)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mul_mod32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mul_mod32.h"

seq_t a(idx_t n)
{
    /* a(n) = 69069^(n-1) mod 2^32.
       For n==1 the exponent is 0 and the result is 1. */
    if (n == 0)            /* undefined, but return 0 for safety */
        return (seq_t)0;

    seq_t result = 1;      /* result = 69069^0 */
    seq_t base   = 69069;  /* multiplier */
    idx_t  exp   = n - 1;  /* exponent */

    while (exp > 0) {
        if (exp & 1)
            result = mul_mod32(result, base);
        base = mul_mod32(base, base);
        exp >>= 1;
    }

    return result;
}
