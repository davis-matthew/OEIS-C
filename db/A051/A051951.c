/*
 * A051951  Second differences of tau(n).
 * https://oeis.org/A051951
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = tau(n+2) - 2*tau(n+1) + tau(n) where tau(m) is the number
 *            of positive divisors of m with tau(0)=0. Implements the second
 *            difference of the divisor function
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 97 terms through n = 9980 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute the divisor function tau(m) for m >= 0.
   tau(0) is defined to be 0. */
static seq_t tau(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t count = 0;
    /* Iterate i from 1 to floor(sqrt(n)) safely with n/i comparison. */
    for (idx_t i = 1; i <= n / i; ++i)
    {
        if (n % i == 0)
        {
            if (i == n / i)   /* perfect square, count divisor once */
                count += 1;
            else              /* pair of distinct divisors */
                count += 2;
        }
    }
    return count;
}

seq_t a(idx_t n)
{
    /* The original problem defines a(0) = 0 assuming tau(0)=0. */
    seq_t t0 = tau(n);
    seq_t t1 = tau(n + 1);
    seq_t t2 = tau(n + 2);
    return t2 - 2 * t1 + t0;
}
