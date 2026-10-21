/*
 * A083245  Difference between numbers of related and numbers of unrelated
 *          numbers belonging to n: a(n) = A073757(n)-A045763(n) =
 *          (n-u(n))-u(n) = n-2*A045763(n) = 2*A073757(n)-n.
 * https://oeis.org/A083245
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 2 * (tau(n) + phi(n) - 1) - n where tau(n) = number of
 *            positive divisors of n phi(n) = Euler's totient function of n
 * Range:     i8 n <= 129, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n=0, return 0 */

    idx_t tmp = n;                /* work on a copy while factoring */
    seq_t tau = (seq_t)1;         /* divisor count starts at 1 */
    seq_t phi = (seq_t)n;         /* totient starts as n */

    /* trial division.  Use p <= tmp/p to avoid overflow on p*p. */
    for (idx_t p = 2; p <= tmp / p; ++p) {
        if (tmp % p == 0) {
            int exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            tau *= (seq_t)(exp + 1);
            phi = phi / (seq_t)p * (seq_t)(p - 1);
        }
    }

    if (tmp > 1) {          /* remaining prime factor */
        tau *= (seq_t)2;
        phi = phi / (seq_t)tmp * (seq_t)(tmp - 1);
    }

    /* a(n) = 2*(tau + phi - 1) - n */
    seq_t result = (seq_t)2 * (tau + phi - (seq_t)1) - (seq_t)n;
    return result;
}
