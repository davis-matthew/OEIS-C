/*
 * A119921  Number of rationals in [0, 1) having at most n preperiodic bits,
 *          then at most n periodic bits.
 * https://oeis.org/A119921
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 2^n * Σ_{j=1..n} Σ_{d|j} (2^d – 1) * μ(j/d)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of data up to the largest n seen */
    static seq_t *pow2 = NULL;
    static int    *mu   = NULL;
    static seq_t *g    = NULL;
    static seq_t *pref = NULL;
    static size_t cache_size = 0;           /* cached indices 0 .. cache_size-1 */

    /* Grow cache if necessary */
    if ((size_t)n >= cache_size) {
        size_t new_cap = (size_t)n + 1;     /* allocate 0 .. n */

        pow2  = (seq_t *)realloc(pow2,  new_cap * sizeof(seq_t));
        mu    = (int   *)realloc(mu,    new_cap * sizeof(int));
        g     = (seq_t *)realloc(g,     new_cap * sizeof(seq_t));
        pref  = (seq_t *)realloc(pref,  new_cap * sizeof(seq_t));

        /* 1️⃣  Pre‑compute powers of 2 */
        if (cache_size == 0)       pow2[0] = (seq_t)1;
        for (size_t i = 1; i < new_cap; ++i)
            pow2[i] = pow2[i-1] * (seq_t)2;

        /* 2️⃣  Compute Möbius function up to n using a linear sieve */
        int  *is_comp = (int  *)calloc(new_cap, sizeof(int));
        int  *prime   = (int  *)malloc (new_cap * sizeof(int));
        size_t pcnt = 0;

        mu[0] = 0;
        if (new_cap > 1) mu[1] = 1;

        for (size_t i = 2; i < new_cap; ++i) {
            if (!is_comp[i]) {
                prime[pcnt++] = (int)i;
                mu[i] = -1;
            }
            for (size_t j = 0; j < pcnt; ++j) {
                size_t p = prime[j];
                size_t v = i * p;
                if (v >= new_cap) break;
                is_comp[v] = 1;
                if (i % p == 0) {          /* p divides i   */
                    mu[v] = 0;
                    break;
                } else {
                    mu[v] = -mu[i];
                }
            }
        }
        free(is_comp);
        free(prime);

        /* 3️⃣  Compute g[j] = Σ_{d|j} (2^d – 1) * μ(j/d) */
        for (size_t i = 0; i < new_cap; ++i) g[i] = (seq_t)0;
        for (size_t d = 1; d < new_cap; ++d) {
            seq_t term = pow2[d] - (seq_t)1;
            for (size_t m = d; m < new_cap; m += d) {
                int mu_val = mu[m / d];
                if (mu_val == 0) continue;
                g[m] += term * (seq_t)mu_val;
            }
        }

        /* 4️⃣  Prefix sums of g */
        pref[0] = (seq_t)0;
        for (size_t i = 1; i < new_cap; ++i)
            pref[i] = pref[i-1] + g[i];

        cache_size = new_cap;
    }

    /* 5️⃣  Result:  a(n) = 2^n * pref[n] */
    return pow2[(size_t)n] * pref[(size_t)n];
}
