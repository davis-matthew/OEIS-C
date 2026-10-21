/*
 * A056606  Squarefree kernel of lcm(binomial(n,0), ..., binomial(n,n)).
 * https://oeis.org/A056606
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = radical(lcm(binomial(n,0), …, binomial(n,n)))
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 28, u64 n <= 54, u128 n <= 102,
 *            big any n
 * Helpers:   seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_mul.h"

/* Compute the squarefree kernel of lcm of all binomial coefficients of row n */
seq_t a(idx_t n)
{
    if (n == 0 || n == 1)
        return 1;   /* lcm = 1 has radical 1 */

    /* 1. Build smallest‑prime‑factor array up to n */
    idx_t *spf = (idx_t *)malloc((n + 1) * sizeof(idx_t));
    if (!spf)
        return 0;
    for (idx_t i = 0; i <= n; ++i)
        spf[i] = 0;
    if (n >= 1)
        spf[1] = 1;

    for (idx_t i = 2; i <= n; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            if (i <= n / i) {               /* avoid overflow of i*i */
                for (idx_t j = i * i; j <= n; j += i)
                    if (spf[j] == 0)
                        spf[j] = i;
            }
        }
    }

    /* 2. Auxiliary tables: exponent in current binomial, and whether prime
       has ever appeared with positive exponent. */
    int *exp   = (int *)calloc(n + 1, sizeof(int));
    bool *found = (bool *)calloc(n + 1, sizeof(bool));
    if (!exp || !found) {
        free(spf);
        free(exp);
        free(found);
        return 0;
    }

    /* 3. Process each binomial C(n,k) for k=0..n-1 */
    for (idx_t k = 0; k < n; ++k) {
        /* add numerator factor (n-k) */
        idx_t m = n - k;
        idx_t primes_buf[32];
        int primes_cnt = 0;
        while (m > 1) {
            idx_t p = spf[m];
            while (m % p == 0) {
                if (primes_cnt < 32)
                    primes_buf[primes_cnt++] = p;
                ++exp[p];
                m /= p;
            }
        }

        /* subtract denominator factor (k+1) */
        idx_t d = k + 1;
        while (d > 1) {
            idx_t p = spf[d];
            while (d % p == 0) {
                --exp[p];
                d /= p;
            }
        }

        /* record primes that appear positively in this binomial */
        for (int i = 0; i < primes_cnt; ++i) {
            idx_t p = primes_buf[i];
            if (!found[p] && exp[p] > 0)
                found[p] = true;
        }
    }

    /* 4. Build the radical: product of all marked primes */
    seq_t result = 1;
    for (idx_t p = 2; p <= n; ++p) {
        if (found[p])
            result = seq_mul(result, (seq_t)p);
    }

    free(spf);
    free(exp);
    free(found);
    return result;
}
