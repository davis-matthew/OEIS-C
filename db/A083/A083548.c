/*
 * A083548  Least common multiple of cototient values of consecutive integers.
 * https://oeis.org/A083548
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm( n - φ(n) , (n+1) - φ(n+1) ) (cototients)
 * Range:     u8 n <= 34, u16 n <= 493, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* cototient of n and n+1 */
    seq_t cot1 = ((seq_t)n) - phi(n);
    seq_t cot2 = ((seq_t)(n + 1)) - phi(n + 1);

    /* if either cototient is zero, the LCM is zero */
    if (cot1 == 0 || cot2 == 0)
        return 0;

    /* lcm(x,y) = x / gcd(x,y) * y */
    seq_t g = gcd(cot1, cot2);
    return (cot1 / g) * cot2;
}
