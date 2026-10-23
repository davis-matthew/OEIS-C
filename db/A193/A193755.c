/*
 * A193755  Number of nXnXn triangular 0..4 arrays with no element equal to
 *          the sum mod 5 of all its neighbors.
 * https://oeis.org/A193755
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 4 a(n) = 2^(n*(n+1)) for n even a(n) = 2^(n*(n+1)) +
 *            4^(n-2)*(n-2)^2 for n odd (n ≥ 3) (Values are exact for every n
 *            ≥ 1. The sequence is OEIS A193755.)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)4;

    /* base 2 power: 2^(n*(n+1)) */
    seq_t pow2 = seq_pow((seq_t)2, n * (n + 1));

    if ((n & 1) == 0)               /* even n */
        return pow2;

    /* odd n ≥ 3: add 4^(n-2)*(n-2)^2 */
    seq_t pow4 = seq_pow((seq_t)4, n - 2);
    seq_t t    = (seq_t)(n - 2);
    seq_t add  = pow4 * t * t;

    return pow2 + add;
}
