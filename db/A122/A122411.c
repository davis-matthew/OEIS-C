/*
 * A122411  a(n) is the sum of primes p for those k's, 2 <= k <= n, where
 *          gcd(k,n) = p^j > 1. (a(1) = 0.)
 * https://oeis.org/A122411
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = φ(n) * Σ_{p|n} p/(p-1) where φ(n) is Euler's totient
 *            function. The sequence is defined as the sum of the prime bases
 *            of the prime‑powers dividing gcd(k,n) for 2 ≤ k ≤ n. The formula
 *            above is derived from a(n) = Σ_{p|n} p^v(n,p) * φ(n / p^v(n,p))
 *            which simplifies to the expression used here...
 * Range:     u8 n <= 142, u16 n <= 8690, u32 n <= 8690, u64 n <= 8690,
 *            u128 n <= 8690, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    uint64_t temp = n;
    seq_t phi = (seq_t)n;          /* start φ(n) = n */
    /* keep distinct primes of n in a small array */
    static uint64_t primes[64];
    int pcount = 0;

    /* trial division to factor n */
    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            primes[pcount++] = p;
            /* remove all copies of p */
            while (temp % p == 0)
                temp /= p;
            /* φ(n) = φ(n) / p * (p-1) */
            seq_t p_seq = (seq_t)p;
            phi = phi / p_seq * (p_seq - 1);
        }
    }
    if (temp > 1) {               /* remaining prime factor */
        primes[pcount++] = temp;
        seq_t p_seq = (seq_t)temp;
        phi = phi / p_seq * (p_seq - 1);
    }

    /* compute the sum Σ φ(n)/(p-1) * p over all distinct primes p|n */
    seq_t sum = 0;
    for (int i = 0; i < pcount; ++i) {
        seq_t p_seq = (seq_t)primes[i];
        sum += (phi / (p_seq - 1)) * p_seq;
    }
    return sum;
}
