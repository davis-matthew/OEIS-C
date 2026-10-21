/*
 * A071188  Largest prime factor of number of divisors of n; a(1)=1.
 * https://oeis.org/A071188
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = largest prime factor of τ(n) (the number of divisors of
 *            n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 1 by definition */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* n is guaranteed ≥ 1 by the sequence definition */

    /* compute τ(n), the number of divisors of n */
    uint64_t nn   = (uint64_t)n;
    uint64_t tau  = 1;
    uint64_t tmp  = nn;

    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            uint64_t exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            tau *= (exp + 1);
        }
    }
    if (tmp > 1) tau *= 2;

    /* largest prime factor of τ(n) */
    if (tau <= 1) return (seq_t)1;          /* τ(1) = 1 gives a(1) = 1 */

    uint64_t m      = tau;
    uint64_t largest = 1;

    /* factor out 2 */
    if ((m & 1) == 0) {
        largest = 2;
        while ((m & 1) == 0) m >>= 1;
    }

    /* odd factors */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            largest = p;
            while (m % p == 0) m /= p;
        }
    }

    if (m > 1) largest = m;                 /* remaining prime > sqrt(tau) */

    return (seq_t)largest;
}
