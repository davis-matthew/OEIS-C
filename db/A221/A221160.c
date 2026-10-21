/*
 * A221160  G.f.: Sum_{n>=0} (4*n+1)^n * x^n / (1 + (4*n+1)*x)^n.
 * https://oeis.org/A221160
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*n+3) * 4^(n-1) * n! for n>0, a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   factorial, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/pow_int.h"     /* pow_int(seq_t base, idx_t exp) */
#include "../utils/factorial.h"   /* factorial(idx_t n) */

seq_t a(idx_t n)
{
    /* Base case */
    if (n == 0)
        return (seq_t)1;

    /* n! */
    seq_t fact = factorial(n);

    /* 4^(n-1) */
    seq_t pow4 = pow_int((seq_t)4, n - 1);

    /* (2*n+3) */
    seq_t mult_const = (seq_t)(2 * n + 3);

    /* Compute the product in stages to avoid intermediate overflow for
     * small seq_t types – the values we test will fit in seq_t. */
    seq_t tmp = fact * pow4;          /* (n! * 4^(n-1))   */
    seq_t result = tmp * mult_const; /* (2*n+3) * 4^(n-1) * n! */

    return result;
}
