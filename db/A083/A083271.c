/*
 * A083271  a(n) = n*tau(n) + 1.
 * https://oeis.org/A083271
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*tau(n) + 1
 * Range:     u8 n <= 35, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* tau(n) is the number of divisors of n. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute tau(n) by prime factorisation. */
    seq_t tau = 1;
    idx_t m = n;

    /* factor 2 separately */
    if (m % 2 == 0) {
        idx_t cnt = 0;
        while (m % 2 == 0) {
            m /= 2;
            cnt++;
        }
        tau *= (cnt + 1);
    }

    /* factor odd primes */
    idx_t p = 3;
    while (p <= m / p) {          /* p*p <= m but avoids overflow */
        if (m % p == 0) {
            idx_t cnt = 0;
            while (m % p == 0) {
                m /= p;
                cnt++;
            }
            tau *= (cnt + 1);
        }
        p += 2;                    /* skip even numbers */
    }

    /* if anything remains, it is a prime factor > sqrt(original n) */
    if (m > 1) {
        tau *= 2;
    }

    /* result: n * tau(n) + 1 */
    return (seq_t)n * tau + 1;
}
