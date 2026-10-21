/*
 * A089812  Expansion of Jacobi theta function q^(-1/8) * (theta_2(q^(1/2)) -
 *          3 * theta_2(q^(9/2))) / 2 in powers of q.
 * https://oeis.org/A089812
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = b(8*n+1) where b is multiplicative: * b(2^e) = 0^e
 *            (irrelevant – 8*n+1 is odd) * b(3^e) = -2 * (1+(-1)^e)/2 (=-2 if
 *            e even>0, 0 if e odd) * b(p^e) = (1+(-1)^e)/2 (for any prime
 *            p>3) * Thus a(n) is 0 unless every prime factor other than 3
 *            occurs with an * even exponent. If 3 occurs with an...
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t m = 8*n + 1;          /* odd integer */
    if (m == 1)                /* product over empty set → 1 */
        return (seq_t)1;

    seq_t result = (seq_t)1;
    idx_t temp = m;

    /* handle factor 3 */
    int exp3 = 0;
    while (temp % 3 == 0) {
        ++exp3;
        temp /= 3;
    }
    if (exp3 > 0) {
        if (exp3 & 1)          /* odd exponent → factor 0 */
            return (seq_t)0;
        /* even positive exponent → multiply by -2 */
        result *= (seq_t)-2;
    }

    /* factor remaining odd numbers */
    idx_t p;
    for (p = 5; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            int exp = 0;
            while (temp % p == 0) {
                ++exp;
                temp /= p;
            }
            if (exp & 1)        /* odd exponent → factor 0 */
                return (seq_t)0;
            /* even exponent → factor 1, no change */
        }
    }

    /* If a prime >3 remains, its exponent is 1 (odd) → zero */
    if (temp > 1)
        return (seq_t)0;

    return result;
}
