/*
 * A218300  E.g.f. A(x) satisfies A( x/(exp(x)*cosh(x)) ) =
 *          exp(2*x)*cosh(2*x).
 * https://oeis.org/A218300
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (k+2)^(n-1) * binomial(n,k) (n ≥ 0, with
 *            a(0) = 1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 25,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/big_pow.h"

static idx_t  max_n   = 0;   /* largest n already computed */
static seq_t *a_cache = NULL;   /* a[0..max_n] */
static size_t a_cap  = 0;       /* allocated capacity of a_cache */

static seq_t *pow_cache = NULL;   /* pow_cache[k] = (k+2)^(curr-1) for current curr */
static size_t pow_cap  = 0;       /* allocated capacity of pow_cache */

seq_t a(idx_t n)
{
    /* a(0) is handled specially – it is always 1 */
    if (n == 0) {
        return (seq_t)1;
    }

    /* Lazy initialise the caches on the first call */
    if (a_cache == NULL) {
        a_cap = 1;
        a_cache = (seq_t *)malloc(a_cap * sizeof(seq_t));
        a_cache[0] = (seq_t)1;
        max_n = 0;

        pow_cap = 1;
        pow_cache = (seq_t *)malloc(pow_cap * sizeof(seq_t));
        pow_cache[0] = (seq_t)1;   /* (0+2)^0 = 1 */
    }

    /* Expand a_cache if necessary */
    if ((size_t)n + 1 > a_cap) {
        a_cap = ((size_t)n + 1) * 2;
        a_cache = (seq_t *)realloc(a_cache, a_cap * sizeof(seq_t));
    }

    /* Expand pow_cache if necessary */
    if ((size_t)n + 1 > pow_cap) {
        pow_cap = ((size_t)n + 1) * 2;
        pow_cache = (seq_t *)realloc(pow_cache, pow_cap * sizeof(seq_t));
    }

    /* Compute terms up to n */
    for (idx_t curr = max_n + 1; curr <= n; ++curr) {
        if (curr == 1) {
            /* (k+2)^(0) = 1 for all k, so we only need to set pow_cache[1] */
            pow_cache[1] = (seq_t)1;
        } else {
            /* Increase exponent by 1 for each existing k < curr */
            for (idx_t k = 0; k < curr; ++k) {
                pow_cache[k] = pow_cache[k] * (seq_t)(k + 2);
            }
            /* Compute the new power for k = curr : (curr+2)^(curr-1) */
            pow_cache[curr] = big_pow((seq_t)(curr + 2),
                                      (seq_t)(curr - 1));
        }

        /* Compute a(curr) = Σ_{k=0..curr} binomial(curr,k) * pow_cache[k] */
        seq_t sum = 0;
        for (idx_t k = 0; k <= curr; ++k) {
            sum += binomial(curr, k) * pow_cache[k];
        }
        a_cache[curr] = sum;
    }

    max_n = n;
    return a_cache[(size_t)n];
}
