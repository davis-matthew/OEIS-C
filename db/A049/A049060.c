/*
 * A049060  a(n) = (-1)^omega(n)*Sum_{d|n} d*(-1)^omega(d), where omega(n) =
 *          A001221(n) is number of distinct primes dividing n.
 * https://oeis.org/A049060
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, nice, mult
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 1086, u32 n <= 9941, u64 n <= 9941,
 *            u128 n <= 9941, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Sum_{d|n} d*(-1)^omega(d)
 *            (multiplicative:  a(p^e) = (p^(e+1)-1)/(p-1) - 2)
 *
 * The function below computes a(n) using prime factorisation of n.
 * It works for any n >= 1, in any order of calls and stays well under
 * 2.0 seconds for n up to about 10000.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1) return 1;          /* a(1) = 1 */

    idx_t m = n;
    seq_t result = 1;

    /* trial division by possible prime factors */
    for (idx_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            idx_t e = 0;
            seq_t pow_p = 1;
            /* find exponent e of prime p in n */
            while (m % p == 0) {
                m /= p;
                ++e;
                pow_p *= (seq_t)p;   /* pow_p = p^e */
            }
            /* compute (p^(e+1)-1)/(p-1) - 2 */
            seq_t pow_pe1 = pow_p * (seq_t)p;        /* p^(e+1) */
            seq_t numerator = pow_pe1 - 1;
            seq_t denominator = (seq_t)(p - 1);
            seq_t term = numerator / denominator - 2;
            result *= term;
        }
    }

    /* Remaining prime factor > sqrt(n) (at most one) */
    if (m > 1) {
        idx_t p = m;          /* exponent e = 1 */
        seq_t pow_p  = (seq_t)p;
        seq_t pow_pe1 = pow_p * (seq_t)p;   /* p^2 */
        seq_t numerator = pow_pe1 - 1;
        seq_t denominator = (seq_t)(p - 1);
        seq_t term = numerator / denominator - 2;
        result *= term;
    }

    return result;
}
