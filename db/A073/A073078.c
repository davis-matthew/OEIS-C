/*
 * A073078  Least k such that n divides C(2k,k).
 * https://oeis.org/A073078
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = min{k >= 1 | n divides binom(2*k, k)}
 * Range:     u8 n <= 511, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* handle trivial case */
    if (n == 1) return 1;

    /* factorization of n */
    idx_t primes[64];
    idx_t exps[64];
    int pcnt = 0;
    idx_t m = n;
    if (m % 2 == 0) {
        primes[pcnt] = 2;
        exps[pcnt] = 0;
        while (m % 2 == 0) {
            m /= 2;
            exps[pcnt]++;
        }
        pcnt++;
    }
    for (idx_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            primes[pcnt] = p;
            exps[pcnt] = 0;
            while (m % p == 0) {
                m /= p;
                exps[pcnt]++;
            }
            pcnt++;
        }
    }
    if (m > 1) {
        primes[pcnt] = m;
        exps[pcnt] = 1;
        pcnt++;
    }

    /* search for minimal k */
    for (idx_t k = 1; ; ++k) {
        bool ok = true;
        for (int i = 0; i < pcnt && ok; ++i) {
            idx_t p   = primes[i];
            idx_t need = exps[i];
            seq_t got = 0;
            idx_t twok = k + k;   /* 2*k */
            idx_t pk = p;

            while (pk <= twok) {
                got += (twok / pk) - 2 * (k / pk);
                if (pk > UINT64_MAX / p) break;  /* avoid overflow */
                pk *= p;
            }
            if (got < need) ok = false;
        }
        if (ok) return (seq_t)k;
    }
}
