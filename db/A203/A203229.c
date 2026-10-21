/*
 * A203229  (n-1)-st elementary symmetric function of (1,16,...,n^4).
 * https://oeis.org/A203229
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=1..n} (n! / k)^4 (elementary symmetric function of
 *            degree n-1 of 1,16,81,…,n^4)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Auxiliary function: compute n! into result (using seq_t arithmetic) */
static void factorial_u(idx_t n, seq_t *res)
{
    *res = 1;
    for (idx_t i = 2; i <= n; ++i)
    {
        *res *= (seq_t)i;          /* seq_t multiplication */
    }
}

seq_t a(idx_t n)
{
    /* n >= 1 */
    seq_t factorial = 1;
    factorial_u(n, &factorial);

    seq_t sum = 0;

    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t q   = factorial / (seq_t)k;     /* integer division */
        seq_t q2  = q * q;                    /* q^2 */
        seq_t q4  = q2 * q2;                  /* q^4 */
        sum += q4;                            /* accumulate */
    }

    return sum;
}
