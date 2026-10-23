/*
 * A174725  a(n) = (A074206(n) + A008683(n))/2.
 * https://oeis.org/A174725
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (A074206(n) + μ(n)) / 2, a(n) = μ(n) + Σ_{d|n,d<n} a(d)
 *            with μ = Möbius function. a(1) = 1 (offset 1). The function is
 *            memoised so calls may be in any order
 * Range:     u8 n <= 287, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    static seq_t *vals = NULL;
    static int   *mu   = NULL;
    static size_t cap   = 0;            /* maximum n that has been computed      */
    static bool  *comp  = NULL;         /* composite flag for sieve              */
    static size_t comp_cap = 0;         /* size of comp                            */
    static bool  *prime_flag = NULL;    /* temporary flag during prime sieve      */
    static size_t pf_cap = 0;           /* size of prime_flag                      */
    static size_t *spf = NULL;          /* smallest prime factor array             */
    static size_t spf_cap = 0;          /* size of spf array                       */
    static int *primes = NULL;          /* array of primes                        */
    static size_t primes_cnt = 0;       /* number of primes in primes array        */

    if (n == 0) return 0;               /* undefined for offset < 1                */

    /* increase capacity if necessary */
    if ((size_t)n > cap) {
        size_t new_cap = (size_t)n;
        /* enlarge value array */
        vals = (seq_t *)realloc(vals, (new_cap + 1) * sizeof(seq_t));
        /* enlarge mu array    */
        mu = (int *)realloc(mu, (new_cap + 1) * sizeof(int));

        /* enlarge composite flag array */
        if (new_cap > comp_cap) {
            comp = (bool *)realloc(comp, (new_cap + 1) * sizeof(bool));
            for (size_t i = comp_cap; i <= new_cap; ++i) comp[i] = false;
            comp_cap = new_cap;
        }

        /* enlarge smallest prime factor array */
        if (new_cap > spf_cap) {
            spf = (size_t *)realloc(spf, (new_cap + 1) * sizeof(size_t));
            for (size_t i = spf_cap; i <= new_cap; ++i) spf[i] = 0;
            spf_cap = new_cap;
        }
        /* resize primes array if necessary */
        if (primes_cnt > new_cap) {
            primes_cnt = 0;   /* clear old primes list         */
            free(primes);
            primes = NULL;
        }
        if (!primes) primes = (int *)malloc((new_cap + 1) * sizeof(int));

        /* compute smallest prime factors (spf) for [1..new_cap] */
        for (size_t i = 2; i <= new_cap; ++i) spf[i] = 0;
        for (size_t i = 2; i <= new_cap; ++i) {
            if (spf[i] == 0) {
                spf[i] = i;
                if (i * i <= new_cap) {
                    for (size_t j = i * i; j <= new_cap; j += i)
                        if (spf[j] == 0) spf[j] = i;
                }
            }
        }

        /* compute Möbius μ[1..new_cap] using spf */
        mu[1] = 1;
        for (size_t i = 2; i <= new_cap; ++i) {
            size_t p = spf[i];
            size_t m = i / p;
            if (spf[m] == p)
                mu[i] = 0;
            else
                mu[i] = -mu[m];
        }

        /* compute a[n] for n from (cap+1) up to new_cap */
        vals[1] = 1;  /* base case */
        for (size_t i = (cap > 0 ? cap + 1 : 2); i <= new_cap; ++i) {
            seq_t sum = 0;
            for (idx_t d = 1; (size_t)(d * d) <= i; ++d) {
                if (i % d == 0) {
                    idx_t d1 = d;
                    idx_t d2 = (idx_t)(i / d);
                    if (d1 < i) sum += vals[d1];
                    if (d2 != d1 && d2 < i) sum += vals[d2];
                }
            }
            if (mu[i] == 1)
                vals[i] = sum + 1;
            else if (mu[i] == 0)
                vals[i] = sum;
            else /* mu[i] == -1 */
                vals[i] = sum - 1;   /* sum ≥ 1 because a[1] = 1 is always a divisor */
        }

        cap = new_cap;
    }

    return vals[(size_t)n];
}
