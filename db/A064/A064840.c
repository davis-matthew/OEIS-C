/*
 * A064840  a(n) = tau(n)*sigma(n).
 * https://oeis.org/A064840
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, nonn
 * Formula:   a(n) = tau(n)*sigma(n)
 * Range:     u8 n <= 23, u16 n <= 719, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   tau(n) -- number of divisors of n
    sigma(n) -- sum of divisors of n
*/

#include <stddef.h>  /* for size_t if needed */

seq_t a(idx_t n)
{
    seq_t tau = 1;          /* product of (e_i+1) over prime factors */
    seq_t sigma = 1;        /* product of (p_i^(e_i+1)-1)/(p_i-1) */

    idx_t x = n;

    /* factor 2 separately for speed */
    if ((x & 1) == 0) {
        int e = 0;
        seq_t pow_p = 1;   /* current power of 2 */
        seq_t sigma_p = 1; /* accumulation of 1 + 2 + ... + 2^e */

        while ((x & 1) == 0) {
            x >>= 1;
            e++;
            pow_p *= (seq_t)2;
            sigma_p += pow_p;
        }
        tau *= (seq_t)(e + 1);
        sigma *= sigma_p;
    }

    /* odd prime factors */
    for (idx_t p = 3; p * p <= x; p += 2) {
        if (x % p == 0) {
            int e = 0;
            seq_t pow_p = 1;
            seq_t sigma_p = 1;

            while (x % p == 0) {
                x /= p;
                e++;
                pow_p *= (seq_t)p;
                sigma_p += pow_p;
            }
            tau *= (seq_t)(e + 1);
            sigma *= sigma_p;
        }
    }

    /* remaining prime factor > sqrt(original n) */
    if (x > 1) {
        seq_t pow_p = (seq_t)x;   /* p^1 */
        seq_t sigma_p = 1 + pow_p;/* 1 + p */
        tau *= (seq_t)2;
        sigma *= sigma_p;
    }

    return tau * sigma;
}
