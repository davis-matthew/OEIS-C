/*
 * A038720  a(n) = (n+3)*n!/2.
 * https://oeis.org/A038720
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+3)*n!/2
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 33,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    /* For n < 1 behaviour is irrelevant */
    if (n == 0)
        return 0;

    seq_t fact = factorial(n);          /* n! */
    seq_t mult = (seq_t)(n + 3);        /* (n+3)  */
    return (mult * fact) / 2;           /* (n+3)*n!/2 */
}
