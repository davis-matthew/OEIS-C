/*
 * A178454  Denominator of n!*Sum((-1)^k/k!, k=0..n)/(n-1)^n.
 * https://oeis.org/A178454
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of n!*∑_{k=0}^n (-1)^k/k! divided by (n-1)^n
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   big_pow, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* For indices below the offset the returned value is irrelevant. */
    if (n < 2) {
        return (seq_t)0;
    }

    /* Compute T_n = n! * Σ_{k=0}^n (-1)^k/k! using the recurrence
       T_0 = 1
       T_i = i*T_{i-1} + (-1)^i   (for i ≥ 1) */
    seq_t T = (seq_t)1;          /* T_0 */
    for (idx_t i = 1; i <= n; ++i) {
        T = (seq_t)i * T;        /* i * T_{i-1} */
        if ((i & 1) == 0) {      /* even i : add 1   */
            T = T + (seq_t)1;
        } else {                 /* odd i  : subtract 1 */
            T = T - (seq_t)1;
        }
    }

    /* Compute power = (n-1)^n. */
    seq_t base  = (seq_t)(n - 1);
    seq_t power = big_pow(base, (seq_t)n);

    /* Reduce the fraction and return the denominator. */
    seq_t g     = gcd(T, power);
    seq_t denom = power / g;

    return denom;
}
