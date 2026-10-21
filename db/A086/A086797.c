/*
 * A086797  Discriminant of the polynomial x^n - x - 1.
 * https://oeis.org/A086797
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 9, i64 n <= 15, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = (n^n + (-1)^n (n-1)^(n-1)) * (-1)^ceil(1 + n/2)
 *
 * This code uses binary exponentiation to compute the powers n^n and (n-1)^(n-1)
 * in O(log n) multiplications on the arbitrary‑precision type seq_t.
 * All intermediate values are kept in seq_t, so the function works for
 * any supported integer width (8, 16, 32, 64, 128 bits or arbitrary precision).
 */

#include <stdint.h>
#include <stdbool.h>

/* Fast binary exponentiation.  exp is an idx_t, base and result are seq_t. */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp)
    {
        if (exp & 1) result *= base;
        exp >>= 1;
        if (exp) base *= base;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Compute n^n */
    seq_t term1 = pow_seq((seq_t)n, n);

    /* Compute (-1)^n * (n-1)^(n-1) ; 0^0 is treated as 1 when n=1 */
    seq_t term2;
    if (n == 1)
    {
        term2 = -1;                    /* (-1)^1 * 0^0 = -1 */
    }
    else
    {
        seq_t pow2   = pow_seq((seq_t)(n - 1), n - 1);   /* (n-1)^(n-1) */
        term2 = (n & 1) ? -pow2 : pow2;                 /* multiply by (-1)^n */
    }

    /* Sum inside bracket */
    seq_t sum = term1 + term2;

    /* Compute the final sign factor (-1)^ceil(1 + n/2).
       For integer n, ceil(1 + n/2) = (n + 3) / 2 (floor division). */
    idx_t sign_exp = (n + 3) / 2;
    seq_t sign = (sign_exp & 1) ? -1 : 1;

    return sum * sign;
}
