/*
 * A104254  a(n) = n^n - (-1)^n*(n+1)!.
 * https://oeis.org/A104254
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n^n - (-1)^n*(n+1)!
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 9, i64 n <= 15, i128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
seq_t a(idx_t n)
{
    /* compute n^n using binary exponentiation */
    seq_t power = 1;
    seq_t base = (seq_t)n;
    idx_t exp = n;
    while (exp > 0) {
        if (exp & 1) power *= base;
        exp >>= 1;
        if (exp) base *= base;
    }

    /* compute (n+1)! */
    seq_t fact = 1;
    for (idx_t i = 2; i <= n + 1; ++i) {
        fact *= (seq_t)i;
    }

    /* apply the sign of (-1)^n */
    if (n & 1) {
        /* n is odd: subtract (-fact) == add fact */
        return power + fact;
    } else {
        /* n is even: subtract fact */
        return power - fact;
    }
}
