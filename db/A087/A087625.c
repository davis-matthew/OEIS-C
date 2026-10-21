/*
 * A087625  Number of primes in the ring Z_n.
 * https://oeis.org/A087625
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{p | n, p < n} phi(n/p) where phi is Euler's totient
 *            function. The sum is taken over all distinct prime divisors p of
 *            n that are strictly less than n. For a prime n the sum is empty,
 *            hence a(n)=0
 * Range:     u8 n <= 269, u16 n <= 8993, u32 n <= 8993, u64 n <= 8993,
 *            u128 n <= 8993, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n <= 1) return 0;               /* a(1) = 0 */

    /* factorise n, recording prime divisors and their exponents */
    idx_t temp = n;
    int primes[64], exps[64], cnt = 0;

    for (idx_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            primes[cnt] = (int)p;
            exps[cnt]  = e;
            ++cnt;
        }
    }
    if (temp > 1) {                     /* remaining prime factor */
        primes[cnt] = (int)temp;
        exps[cnt]   = 1;
        ++cnt;
    }

    /* n is prime if it has one prime factor of exponent 1 */
    if (cnt == 1 && exps[0] == 1) return 0;

    /* compute phi(n) */
    seq_t phi = (seq_t)n;
    for (int i = 0; i < cnt; ++i)
        phi -= phi / (seq_t)primes[i];  /* phi *= (p-1)/p */

    /* accumulate the sum */
    seq_t sum = 0;
    for (int i = 0; i < cnt; ++i) {
        idx_t p   = (idx_t)primes[i];
        int   e   = exps[i];
        seq_t term;
        if (e >= 2)          term = phi / (seq_t)p;
        else                term = phi / (seq_t)(p - 1);
        sum += term;
    }
    return sum;
}
