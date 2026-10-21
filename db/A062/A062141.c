/*
 * A062141  Third column sequence of coefficient triangle A062137 of
 *          generalized Laguerre polynomials n!*L(n,3,x).
 * https://oeis.org/A062141
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+2)! * binomial(n+5,5) / 2
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 28,
 *            big any n
 * Helpers:   binomial, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factorial.h"
#include "../utils/binomial.h"

/* External linkage as required */
seq_t a(idx_t n)
{
    /* factorial(n+2) * binomial(n+5,5) / 2 */
    seq_t fact   = factorial(n + 2);   /* (n+2)!  */
    seq_t choose = binomial(n + 5, 5); /* C(n+5,5) */
    seq_t res = fact * choose;         /* product */
    res = res / (seq_t)2;              /* divide by 2! */
    return res;
}
