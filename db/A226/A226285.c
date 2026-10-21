/*
 * A226285  a(n) = [n/2]!*[(n+1)/2]!*C([n/2],4)*C([(n+1)/2],4).
 * https://oeis.org/A226285
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [n/2]! * [(n+1)/2]! * C([n/2],4) * C([(n+1)/2],4)
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 35,
 *            big any n
 * Helpers:   binom, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/factorial.h"
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* floor(n/2) and ceil(n/2) */
    idx_t low  = n / 2;          /* floor(n/2) */
    idx_t high = n - low;        /* ceil(n/2) */

    /* Compute the product of factorials and binomial coefficients */
    seq_t result = factorial(low);
    result *= factorial(high);
    result *= binom(low, 4);
    result *= binom(high, 4);

    return result;   /* returns 0 automatically for n < 8 because one binomial is 0 */
}
