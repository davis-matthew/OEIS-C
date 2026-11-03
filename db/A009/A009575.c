/*
 * A009575  E.g.f. sinh(log(1+x))*log(1+x).
 * https://oeis.org/A009575
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = ((-1)^n*(n!*H_n + (n-2)!))/2 for n>=2, a(0)=a(1)=0 where
 *            H_n = Sum_{k=1..n} 1/k. The product n!*H_n = Sum_{k=1..n} n!/k
 *            is an integer, so the whole expression is integral
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 12, i64 n <= 20, i128 n <= 33,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 439 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Helper: compute factorials iteratively.
   For built‑in integer types the result may overflow for large n,
   but the tests use only n for which the value fits in seq_t. */
static seq_t factorial(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 2; i <= n; ++i)
        result *= (seq_t)i;
    return result;
}

seq_t a(idx_t n) {
    if (n < 2) return 0;          /* a(0) = a(1) = 0 */

    seq_t fac_n          = factorial(n);      /* n! */
    seq_t fac_n_minus_2  = factorial(n - 2);  /* (n-2)! */

    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        sum += fac_n / (seq_t)k;               /* n!/k   (k divides n!) */
    }

    seq_t t = sum + fac_n_minus_2;             /* n!*H_n + (n-2)! */

    if (n & 1) t = -t;                         /* multiply by (-1)^n */

    return t / 2;                              /* division is exact */
}
