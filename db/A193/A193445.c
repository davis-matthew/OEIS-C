/*
 * A193445  a(n) = n! * Sum_{d|n} H(d)*H(n/d), where H(n) is the n-th harmonic
 *          number.
 * https://oeis.org/A193445
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   seq_gcd, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A193445
    a(n) = n! * Sum_{d|n} H(d) * H(n/d)
    where H(k) = 1 + 1/2 + ... + 1/k
    Formula implemented exactly (no hard‑coded terms)    */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/seq_mul.h"   /* seq_mul(a,b) */
#include "../utils/seq_gcd.h"   /* seq_gcd(a,b) */

static inline void reduce_frac(seq_t *num, seq_t *den)
{
    seq_t g = seq_gcd(*num, *den);
    if (g > 1)
    {
        *num = *num / g;
        *den = *den / g;
    }
}

static inline void add_frac(seq_t *num, seq_t *den,
                            seq_t an, seq_t ad)
{
    seq_t g = seq_gcd(*den, ad);
    /* lcm = (*den / g) * ad   is the new denominator   */
    seq_t new_den = (*den / g) * ad;
    /* new numerator  = (*num) * (ad / g) + an * (*den / g) */
    seq_t term1 = *num * (ad / g);
    seq_t term2 = an * (*den / g);
    seq_t new_num = term1 + term2;
    /* reduce finally */
    reduce_frac(&new_num, &new_den);
    *num = new_num;
    *den = new_den;
}

/* factorial, harmonic numerator and denominator caches   */
static idx_t cache_n = 0;
static seq_t *fact  = NULL;   /* fact[i] = i! for i between 0 and cache_n          */
static seq_t *numH  = NULL;   /* harmonic numerator                               */
static seq_t *denH  = NULL;   /* harmonic denominator                             */

static void ensure_cache(idx_t n)
{
    if (n <= cache_n) return;

    size_t new_size = n + 1;
    seq_t *nf = (seq_t*)realloc(fact,  new_size * sizeof(seq_t));
    seq_t *nn = (seq_t*)realloc(numH,  new_size * sizeof(seq_t));
    seq_t *dd = (seq_t*)realloc(denH,  new_size * sizeof(seq_t));

    if (!nf || !nn || !dd) { free(fact); free(numH); free(denH); exit(1); }

    fact = nf;  numH = nn;  denH = dd;

    if (cache_n == 0) /* initialise with n=0, H(0)=0/1   */
    {
        fact[0] = 1;
        numH[0] = 0;
        denH[0] = 1;
        cache_n = 0;
    }

    for (idx_t i = cache_n + 1; i <= n; ++i)
    {
        /* factorial */
        fact[i] = fact[i-1] * (seq_t)i;

        /* H(i) = H(i-1) + 1/i   (store reduced fraction) */
        seq_t new_num = numH[i-1] * (seq_t)i + denH[i-1];
        seq_t new_den = denH[i-1] * (seq_t)i;
        reduce_frac(&new_num, &new_den);
        numH[i] = new_num;
        denH[i] = new_den;
    }
    cache_n = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined but harmless */

    ensure_cache(n);

    /* sum_{d|n} H(d)*H(n/d)   (as a reduced fraction) */
    seq_t s_num = 0;
    seq_t s_den = 1;

    for (idx_t d = 1; d*d <= n; ++d)
    {
        if (n % d == 0)
        {
            idx_t e = n / d;               /* e = n/d */

            /* H(d) * H(e)  = (numH[d]*numH[e]) / (denH[d]*denH[e])   but
               we must reduce first to keep numbers small */
            seq_t g1 = seq_gcd(numH[d], denH[e]);
            seq_t g2 = seq_gcd(numH[e], denH[d]);

            seq_t prod_num = (numH[d]/g1) * (numH[e]/g2);
            seq_t prod_den = (denH[d]/g2) * (denH[e]/g1);
            reduce_frac(&prod_num, &prod_den);

            /* add to sum  (two times if d != e) */
            add_frac(&s_num, &s_den, prod_num, prod_den);
            if (d != e)
                add_frac(&s_num, &s_den, prod_num, prod_den);
        }
    }

    /* final result = fact[n] * s_num / s_den
       cancel gcd between fact[n] and s_den first */

    seq_t g = seq_gcd(fact[n], s_den);
    seq_t fact_part = fact[n] / g;
    seq_t den_part  = s_den   / g;

    /* cancel possible remaining gcd between s_num and den_part */
    g = seq_gcd(s_num, den_part);
    seq_t num_part = s_num / g;
    den_part = den_part / g;

    /* compute numerator: fact_part * num_part   (exact integer) */
    seq_t numerator = fact_part * num_part;

    /* final exact division (den_part divides numerator) */
    seq_t result = numerator / den_part;

    return result;
}
