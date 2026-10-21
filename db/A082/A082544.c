/*
 * A082544  Number of ordered quintuples (a,b,c,d,e) with gcd(a,b,c,d,e)=1
 *          (1<= {a,b,c,d,e} <= n).
 * https://oeis.org/A082544
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1..n} μ(k) * ⌊n/k⌋⁵
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 85, u64 n <= 7183,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>


/* static storage for Möbius values */
static int8_t   *mu    = NULL;  /* mobius values (μ(0) unused) */
static unsigned char *comp = NULL; /* helper array for sieve */
static size_t mu_cap = 0;        /* current allocated size */

/* compute a(n) */
seq_t a(idx_t n)
{
    /* grow arrays if needed */
    if ((size_t)n + 1 > mu_cap) {
        size_t new_cap = (size_t)n + 1;
        int8_t   *tmp_mu = (int8_t*)realloc(mu, new_cap * sizeof(int8_t));
        unsigned char *tmp_comp = (unsigned char*)realloc(comp, new_cap * sizeof(unsigned char));
        if (!tmp_mu || !tmp_comp) return 0;            /* allocation failure */

        mu    = tmp_mu;
        comp  = tmp_comp;
        mu_cap = new_cap;

        /* initialise arrays */
        memset(mu,   0, mu_cap * sizeof(int8_t));
        memset(comp, 0, mu_cap * sizeof(unsigned char));

        if (mu_cap > 1) mu[1] = 1;                    /* μ(1) = 1 */

        /* build μ up to mu_cap-1 */
        for (size_t i = 2; i < mu_cap; ++i) mu[i] = 1; /* initial value */
        for (size_t i = 2; i < mu_cap; ++i) {
            if (!comp[i]) {                           /* i is prime */
                for (size_t j = i; j < mu_cap; j += i) {
                    comp[j] = 1;
                    mu[j]   = -mu[j];
                }
                size_t sq = i * i;
                if (sq < mu_cap) {
                    for (size_t j = sq; j < mu_cap; j += sq)
                        mu[j] = 0;
                }
            }
        }
    }

    /* Möbius summation */
    seq_t pos = 0, neg = 0;
    for (idx_t k = 1; k <= n; ++k) {
        int8_t mk = mu[(size_t)k];
        if (!mk) continue;
        seq_t t  = (seq_t)(n / k);          /* ⌊n/k⌋ */
        seq_t t2 = t * t;                   /* t² */
        seq_t t4 = t2 * t2;                 /* t⁴ */
        seq_t t5 = t4 * t;                  /* t⁵ */
        if (mk > 0) pos += t5;
        else        neg += t5;
    }
    return pos - neg;
}
