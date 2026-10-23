/*
 * A137749  Number of circular permutations of the multiset
 *          {1,1,2,2,...,2n,2n} (up to rotations) with even distances between
 *          equal elements.
 * https://oeis.org/A137749
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((2n)!^3 / n!^2 / 2^(2n) + (2n)! ) / (4n)
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 10,
 *            big any n
 * Helpers:   factorial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factorial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Undefined behavior for n == 0; return 0 for safety */
    if (n == 0) return (seq_t)0;

    const idx_t m = (idx_t)(2 * n);          /* m = 2n */
    seq_t f = factorial(m);                  /* f = (2n)! */
    seq_t g = factorial(n);                  /* g = n! */

    /* t = ((2n)! / n!)^2 / 2^(2n) * (2n)!  */
    seq_t u = f / g;                          /* u = (2n)! / n! */
    seq_t t = u * u;                          /* t = ((2n)! / n!)^2 */
    t = t / pow2(m);                         /* divide by 2^(2n) */
    t = t * f;                               /* multiply by (2n)! */

    seq_t numerator = t + f;                  /* numerator = t + (2n)! */
    seq_t denom = (seq_t)4 * n;               /* denom = 4n */

    return numerator / denom;
}
