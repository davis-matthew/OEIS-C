/*
 * A023849  Sum of exponents in prime-power factorization of binomial(5n,
 *          2n-2).
 * https://oeis.org/A023849
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σp≤5n (e_p(5n)! – e_p((2n−2)!) – e_p((3n+2)!)) where
 *            e_p(m!) is the exponent of prime p in m! (Legendre’s formula)
 * Range:     u8 n <= 487, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/build_primes.h"

seq_t a(idx_t n)
{
    /* Cache primes up to 5*n; the cache grows only when a larger n is
       requested. */
    static idx_t *primes = NULL;
    static size_t primes_cnt = 0;
    static idx_t prime_limit = 0;

    idx_t limit = 5 * n;
    if (limit > prime_limit) {
        if (primes) {
            free(primes);
            primes = NULL;
            primes_cnt = 0;
        }
        build_primes(limit, &primes, &primes_cnt);
        prime_limit = limit;
    }

    idx_t N = 5 * n;
    idx_t K = 2 * n - 2;   /* zero when n==1 */
    idx_t M = 3 * n + 2;

    seq_t total = 0;

    for (size_t i = 0; i < primes_cnt; ++i) {
        idx_t p = primes[i];
        if (p > N) break;

        idx_t eN = 0, eK = 0, eM = 0;

        /* Legendre’s formula for N! */
        idx_t pow = p;
        while (pow <= N) {
            eN += N / pow;
            if (pow > N / p) break;
            pow *= p;
        }

        /* Legendre’s formula for K! */
        pow = p;
        while (pow <= K) {
            eK += K / pow;
            if (pow > K / p) break;
            pow *= p;
        }

        /* Legendre’s formula for M! */
        pow = p;
        while (pow <= M) {
            eM += M / pow;
            if (pow > M / p) break;
            pow *= p;
        }

        seq_t e = (seq_t)(eN - eK - eM);   /* e is non‑negative */
        total += e;
    }

    return total;
}
