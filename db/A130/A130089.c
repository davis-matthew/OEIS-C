/*
 * A130089  a(n) = denominator of Product_{k=1..n} k^mu(n+1-k), where mu(k) =
 *          A008683(k).
 * https://oeis.org/A130089
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = denominator of Π_{k=1}^{n} k^{μ(n+1‑k)} where μ is the
 *            Möbius function
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   build_spf, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/build_spf.h"

seq_t a(idx_t n)
{
    static idx_t *spf = NULL;
    static size_t spf_size = 0;
    static int *mu  = NULL;
    static size_t mu_size = 0;

    /* Ensure the smallest‑prime‑factor array is large enough. */
    if (n >= spf_size) {
        if (spf != NULL) free(spf);
        if (mu  != NULL) free(mu);

        build_spf(n, &spf);        /* spf[i] stores the smallest prime factor of i, 0 and 1 left unspecified. */
        spf_size = n + 1;

        mu = (int*)malloc((n + 1) * sizeof(int));
        mu_size = n + 1;
        mu[1] = 1;

        for (idx_t i = 2; i <= n; ++i) {
            idx_t p = spf[i];
            idx_t m = i / p;
            if (m % p == 0)
                mu[i] = 0;                 /* squared prime factor -> μ=0 */
            else
                mu[i] = -mu[m];           /* add a distinct prime factor */
        }
    } else {
        /* If we already have spf big enough but mu not computed up to n, extend mu. */
        if (mu_size <= n) {
            int *new_mu = (int*)realloc(mu, (n + 1) * sizeof(int));
            if (!new_mu) return (seq_t)0;   /* allocation failure; unlikely for small n */
            mu = new_mu;
            for (idx_t i = mu_size; i <= n; ++i) {
                idx_t p = spf[i];
                idx_t m = i / p;
                if (m % p == 0)
                    mu[i] = 0;
                else
                    mu[i] = -mu[m];
            }
            mu_size = n + 1;
        }
    }

    /* Compute the reduced fraction product. */
    seq_t num = (seq_t)1;
    seq_t den = (seq_t)1;

    for (idx_t k = 1; k <= n; ++k) {
        int e = mu[n + 1 - k];
        seq_t kk = (seq_t)k;

        if (e == 1) {                       /* multiply into numerator */
            seq_t g = gcd(den, kk);
            den /= g;
            kk  /= g;
            num *= kk;
        } else if (e == -1) {               /* multiply into denominator */
            seq_t g = gcd(num, kk);
            num /= g;
            kk  /= g;
            den *= kk;
        }
        /* exponent 0: skip */
    }

    /* Final reduction, if any. */
    seq_t g = gcd(num, den);
    den /= g;
    return den;
}
