/*
 * A094950  Number of integer coefficients in n-th Bernoulli polynomial
 *          (including zeros).
 * https://oeis.org/A094950
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer coefficients of the n-th Bernoulli
 *            polynomial B_n(x) (including the zero coefficients).
 *            Implementation uses the explicit formula B_n(x) = Σ_{k=0}^n
 *            C(n,k) * B_k * x^{n-k} where B_k are the Bernoulli numbers. Only
 *            the primes occurring in the denominator of B_k (von Staudt–...
 * Range:     u8 n <= 299, u16 n <= 5160, u32 n <= 5160, u64 n <= 5160,
 *            u128 n <= 5160, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* Special case n = 0 */
    if (n == 0) return (seq_t)1;

    /* Precompute primes and smallest prime factor up to n + 1 */
    idx_t limit = n + 1;
    int *spf = (int*)malloc((limit + 1) * sizeof(int));
    for (idx_t i = 0; i <= limit; ++i) spf[i] = (int)i;
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (spf[i] == (int)i) {
            for (idx_t j = i * i; j <= limit; j += i)
                if (spf[j] == (int)j) spf[j] = (int)i;
        }
    }

    /* Count primes and build the prime list and index map */
    int prime_cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (spf[i] == (int)i) ++prime_cnt;

    int *primes = (int*)malloc(prime_cnt * sizeof(int));
    int *prime_index = (int*)malloc((limit + 1) * sizeof(int));
    for (idx_t i = 0; i <= limit; ++i) prime_index[i] = -1;

    int pos = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (spf[i] == (int)i) {
            primes[pos] = (int)i;
            prime_index[i] = pos;
            ++pos;
        }

    /* Exponent array for current binomial coefficient */
    int *exps = (int*)calloc(prime_cnt, sizeof(int));

    seq_t count = 0;

    for (idx_t k = 0; k <= n; ++k) {
        /* Update binomial coefficient exponents */
        if (k > 0) {
            int num = (int)(n - k + 1);   /* numerator */
            int den = (int)k;             /* denominator */

            /* factorize numerator */
            while (num > 1) {
                int p = spf[num];
                int idx = prime_index[p];
                while (num % p == 0) {
                    num /= p;
                    exps[idx] += 1;
                }
            }
            /* factorize denominator */
            while (den > 1) {
                int p = spf[den];
                int idx = prime_index[p];
                while (den % p == 0) {
                    den /= p;
                    exps[idx] -= 1;
                }
            }
        }

        /* Determine if coefficient is an integer */
        bool is_int = false;
        if (k == 0) {
            is_int = true;                      /* B_0 term */
        } else if (k == 1) {
            is_int = ((n & 1) == 0);            /* -n/2 integer only if n even */
        } else if ((k & 1) && k > 1) {
            is_int = true;                      /* B_k = 0 for odd k>1 */
        } else {
            /* k is even, k >= 2: check denominator primes of B_k */
            int two_m = (int)k; /* k = 2m */
            bool ok = true;
            for (int i = 0; i < prime_cnt; ++i) {
                int p = primes[i];
                if (two_m % (p - 1) == 0) {      /* p divides the denominator of B_k */
                    if (exps[i] == 0) {          /* p not in binomial coefficient */
                        ok = false;
                        break;
                    }
                }
            }
            is_int = ok;
        }

        if (is_int) ++count;
    }

    /* Free allocated memory */
    free(spf);
    free(primes);
    free(prime_index);
    free(exps);

    return (seq_t)count;
}
