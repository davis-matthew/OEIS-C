/*
 * A238538  A fourth-order linear divisibility sequence: a(n) = (2^n +
 *          1)*(2^(3*n) - 1)/ ( (2 + 1)*(2^3 - 1) ).
 * https://oeis.org/A238538
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2^n + 1)*(2^(3*n) - 1) / 21 (n ≥ 1)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 33,
 *            big any n
 * Helpers:   gcd, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* The sequence is only defined for n ≥ 1.  Returning 0 for n == 0
       keeps the function defined for all idx_t values. */
    if (n == 0) return 0;

    /* Compute the two powers separately.  (2^n + 1) and (2^(3n) - 1). */
    seq_t two_n  = pow2(n);          /* 2^n  */
    seq_t two_3n = pow2(3 * n);      /* 2^(3n) */

    seq_t A = two_n + (seq_t)1;
    seq_t B = two_3n - (seq_t)1;

    /* Reduce the product by factoring out 21 early to avoid overflow. */
    const seq_t divisor = (seq_t)21;

    /* Divide out common factors of 21 from A first. */
    seq_t g1 = gcd(A, divisor);
    seq_t Ared = A / g1;
    seq_t rem  = divisor / g1;      /* remainder of the divisor to cancel later */

    /* Now divide out whatever is left of the divisor from B. */
    seq_t g2 = gcd(B, rem);
    seq_t Bred = B / g2;
    seq_t rem2 = rem / g2;          /* after this rem2 should be 1 */

    /* Now Ared * Bred is exact and fits in seq_t for all n whose a(n)
       fits; the final division by rem2 (which will be 1) is included
       for completeness. */
    return (Ared * Bred) / rem2;
}
