/*
 * A211261  Number of integer pairs (x,y) such that 0<x<y<=n and x*y=2n.
 * https://oeis.org/A211261
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(d(2n)/2) - 1, where d(m) = number of divisors of m.
 *            d(2n) is computed from the prime factorisation of n
 * Range:     u8 n <= 9765, u16 n <= 9765, u32 n <= 9765, u64 n <= 9765,
 *            u128 n <= 9765, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Count the exponent of 2 in n */
    idx_t k = 0;
    idx_t tmp = n;
    while ((tmp & 1) == 0) {
        tmp >>= 1;
        k++;
    }

    /* tmp is now the odd part of n */
    idx_t m = tmp;
    seq_t tau_m = 1;                /* number of divisors of m */

    idx_t p = 3;
    while (p <= m / p) {
        if (m % p == 0) {
            idx_t exp = 0;
            while (m % p == 0) {
                m /= p;
                exp++;
            }
            tau_m *= (seq_t)(exp + 1);
        }
        p += 2;                     /* skip even numbers */
    }
    if (m > 1)                      /* remaining prime factor */
        tau_m *= 2;

    /* Number of divisors of 2n */
    seq_t tau_2n = tau_m * (seq_t)(k + 2);

    /* a(n) = floor(tau_2n / 2) - 1 */
    seq_t result = tau_2n / 2;
    result -= 1;

    return result;
}
