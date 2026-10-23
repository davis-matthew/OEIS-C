/*
 * A117108  Moebius transform of tetrahedral numbers.
 * https://oeis.org/A117108
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 39, u32 n <= 2459, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A117108: a(n) = Σ_{d|n} μ(d) * T(n/d),
    where μ is the Möbius function and
    T(m) = binomial(m+2,3) = m*(m+1)*(m+2)/6          */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/floor_isqrt_u64.h"

/* Tetrahedral number T(m) = m*(m+1)*(m+2)/6. */
static inline seq_t tetrahedral(idx_t m)
{
    seq_t sm = (seq_t)m;
    seq_t t  = sm * (sm + 1) * (sm + 2);
    t /= 6;
    return t;
}

/* Build Möbius values μ[0..limit] (μ[0] is unused). */
static void compute_mu_upto(idx_t limit, int *mu)
{
    /* initialise μ[i] = 1 for all i; μ[0] = 0, μ[1] = 1 */
    for (idx_t i = 0; i <= limit; ++i) mu[i] = 1;
    mu[0] = 0;
    mu[1] = 1;

    bool *isComp = (bool*)calloc(limit + 1, sizeof(bool));
    if (!isComp) return;       /* should not happen for the sizes we use */

    for (idx_t i = 2; i <= limit; ++i)
    {
        if (!isComp[i])          /* i is prime */
        {
            for (idx_t j = i; j <= limit; j += i)
            {
                mu[j] = -mu[j];
                isComp[j] = true;   /* mark composite */
            }
            if (i * i <= limit)
            {
                for (idx_t j = i * i; j <= limit; j += i * i)
                    mu[j] = 0;      /* square factor → μ = 0 */
            }
        }
    }
    free(isComp);
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* off‑by‑one guard; offset is 1 */

    /* Memoised Möbius array up to the largest n requested so far. */
    static int *mu = NULL;
    static idx_t mu_max = 0;
    if (n > mu_max)
    {
        free(mu);
        mu = (int*)calloc(n + 1, sizeof(int));
        compute_mu_upto(n, mu);
        mu_max = n;
    }

    seq_t pos = 0;
    seq_t neg = 0;

    idx_t sq = floor_isqrt_u64(n);
    for (idx_t d = 1; d <= sq; ++d)
    {
        if (n % d == 0)
        {
            idx_t e = n / d;

            /* Contribution from divisor d (μ(d) * T(e)) */
            int mu_d = mu[d];
            if (mu_d != 0)
            {
                seq_t t = tetrahedral(e);
                if (mu_d == 1) pos += t; else neg += t;
            }

            /* Contribution from paired divisor e, if distinct */
            if (e != d)
            {
                int mu_e = mu[e];
                if (mu_e != 0)
                {
                    seq_t t = tetrahedral(d);  /* T(n/e) = T(d) */
                    if (mu_e == 1) pos += t; else neg += t;
                }
            }
        }
    }

    return pos - neg;   /* guaranteed non‑negative */
}
