/*
 * A060841  Numerator of 1/det(M) where M is the n X n matrix with M[i,j] =
 *          1/lcm(i,j).
 * https://oeis.org/A060841
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n!)^2 / (phi(1)*phi(2)*...*phi(n))
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n) {
    if (n < 1) return 0;                 /* offset 1 */

    /* Sieve for smallest prime factor */
    idx_t N = n;
    idx_t *spf = (idx_t*)calloc(N + 1, sizeof(idx_t));
    if (!spf) return 0;                  /* allocation failure */

    /* dynamic array for primes */
    size_t primes_cap = N / 10 + 1;
    idx_t *primes = (idx_t*)malloc(primes_cap * sizeof(idx_t));
    size_t primes_sz = 0;

    for (idx_t i = 2; i <= N; ++i) {
        if (!spf[i]) {
            spf[i] = i;
            if (primes_sz == primes_cap) {
                primes_cap *= 2;
                primes = (idx_t*)realloc(primes, primes_cap * sizeof(idx_t));
            }
            primes[primes_sz++] = i;
        }
        for (size_t j = 0; j < primes_sz; ++j) {
            idx_t p = primes[j];
            if (p > spf[i] || i * p > N) break;
            spf[i * p] = p;
        }
    }

    /* Exponent table for primes */
    int *exp = (int*)calloc(N + 1, sizeof(int));
    if (!exp) { free(spf); free(primes); return 0; }

    for (idx_t k = 2; k <= N; ++k) {
        /* factor k^2: add 2 * exponents of k */
        idx_t tmp = k;
        while (tmp > 1) {
            idx_t p = spf[tmp];
            int cnt = 0;
            do { tmp /= p; ++cnt; } while (tmp % p == 0);
            exp[p] += 2 * cnt;
        }

        /* compute phi(k) and subtract its factorization */
        seq_t phi = k;
        tmp = k;
        while (tmp > 1) {
            idx_t p = spf[tmp];
            while (tmp % p == 0) tmp /= p;
            phi = phi / p * (p - 1);
        }

        /* factorize phi and subtract exponents */
        idx_t phi_int = (idx_t)phi;          /* fits into idx_t for k <= 10000 */
        tmp = phi_int;
        while (tmp > 1) {
            idx_t p = spf[tmp];
            int cnt = 0;
            while (tmp % p == 0) { tmp /= p; ++cnt; }
            exp[p] -= cnt;
        }
    }

    seq_t result = 1;
    for (idx_t p = 2; p <= N; ++p) {
        int e = exp[p];
        for (int i = 0; i < e; ++i) result *= p;
    }

    free(spf);
    free(primes);
    free(exp);
    return result;
}
