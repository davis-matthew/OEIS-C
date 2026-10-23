/*
 * A152772  a(n) = sigma(n) - 3*d(n) + 3.
 * https://oeis.org/A152772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sigma(n) - 3*d(n) + 3
 * Range:     u8 n <= 48, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where sigma(n) is the sum of divisors and d(n) is the number of divisors. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* n is guaranteed to be >= 1 for all calls that matter. */
    uint64_t m = (uint64_t)n;          /* work on a 64-bit integer */
    seq_t sigma = 1;                  /* product over divisor sums of prime powers */
    seq_t dcount = 1;                 /* product over (exponent + 1) */

    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            uint64_t exp = 0;
            uint64_t p_pow = 1;        /* p^exp */

            while (m % p == 0) {
                m /= p;
                ++exp;
                p_pow *= p;
            }

            /* sum of divisors for this prime power: (p^(exp+1)-1)/(p-1) */
            seq_t p_pow_next = (seq_t)p_pow * (seq_t)p;          /* p^(exp+1) */
            seq_t term = (p_pow_next - 1) / (p - 1);
            sigma *= term;

            dcount *= (exp + 1);
        }
    }

    /* If a prime factor > sqrt(n) remains. */
    if (m > 1) {
        /* m is prime with exponent 1 */
        sigma *= (seq_t)(m + 1);       /* m^1 + m^0 = m + 1 */
        dcount *= 2;                  /* exponent 1 => (1+1) */
    }

    /* a(n) = sigma(n) - 3*d(n) + 3 */
    seq_t result = sigma - 3 * dcount + 3;
    return result;
}
