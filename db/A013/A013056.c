/*
 * A013056  exp(sinh(x)+arctan(x))=1+2*x+4/2!*x^2+7/3!*x^3+8/4!*x^4+17/5!*x^5...
 * https://oeis.org/A013056
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 13, i64 n <= 20, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Recurrence for A013056:
   a(0) = 1
   For n ≥ 1:
       a(n) = ∑_{k=0}^{n-1} C(n-1, k) · a(k) · c(n-1-k)
   where C(n-1, k) are binomial coefficients and
   c(m) = { 1 + m!·(−1)^{m/2}   if m is even
            0                     if m is odd }.
   This follows from the EGF identity  
       derivative( exp(g(x)) ) = exp(g(x))·g'(x)   with g(x)=sinh(x)+atan(x).
*/

#include <stddef.h>
#include <stdlib.h>

static seq_t *a_cache   = NULL;   /* cache of computed a(n) values            */
static seq_t *fac_cache = NULL;   /* factorials m! for m ≤ current limit      */
static seq_t *c_cache   = NULL;   /* c(m) values as above                     */
static size_t  cache_cap = 0;     /* allocated capacity of the arrays         */
static size_t  cache_len = 0;     /* number of a(n) terms that have been computed (a(0) … a(cache_len-1)) */

/* ensure that a_cache[0 … n] and the supporting tables are available */
static void ensure_size(idx_t n)
{
    idx_t const needed = n;              /* we need terms up to a(n) */
    if (cache_len > needed) return;     /* already computed */

    /* expand allocation if necessary */
    size_t new_cap = cache_cap ? cache_cap : 4;
    while (new_cap <= (size_t)needed) new_cap <<= 1;

    /* reallocate arrays to new_cap */
    if (cache_cap == 0) {
        a_cache   = (seq_t *)malloc(new_cap * sizeof(seq_t));
        fac_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        c_cache   = (seq_t *)malloc(new_cap * sizeof(seq_t));
    } else {
        a_cache   = (seq_t *)realloc(a_cache,   new_cap * sizeof(seq_t));
        fac_cache = (seq_t *)realloc(fac_cache, new_cap * sizeof(seq_t));
        c_cache   = (seq_t *)realloc(c_cache,   new_cap * sizeof(seq_t));
    }

    /* initialise new factorials and c-values */
    for (idx_t i = cache_cap; i < new_cap; ++i) {
        if (i == 0) {
            fac_cache[0] = (seq_t)1;
            c_cache[0]   = (seq_t)2;          /* 1 + 0! = 2  */
        } else {
            fac_cache[i] = fac_cache[i-1] * (seq_t)i;   /* m! = (m-1)!·m */

            if ((i & 1) == 0) {                      /* even i */
                seq_t sign = (((i / 2) & 1) ? (seq_t)-1 : (seq_t)1);
                c_cache[i] = (seq_t)1 + fac_cache[i] * sign;
            } else { /* odd i */ 
                c_cache[i] = (seq_t)0;
            }
        }
    }

    /* compute missing a(n) values */
    for (idx_t ncur = cache_len; ncur <= needed; ++ncur) {
        if (ncur == 0) {               /* a(0) = 1 */
            a_cache[0] = (seq_t)1;
            continue;
        }
        idx_t const n1 = ncur - 1;       /* shortcut for C(n1, k) */
        seq_t sum = (seq_t)0;
        seq_t bin = (seq_t)1;            /* binomial coefficient C(n1,0) */

        for (idx_t k = 0; k <= n1; ++k) {
            idx_t m = n1 - k;            /* m = n-1-k */
            if ((m & 1) == 0) {          /* c(m) exists only for even m */
                seq_t term = bin * a_cache[k];
                term = term * c_cache[m];
                sum += term;
            }
            /* update binomial coefficient C(n1, k+1) for next iteration */
            if (k < n1) {
                seq_t num = bin * (seq_t)(n1 - k);
                bin = num / (seq_t)(k + 1);
            }
        }
        a_cache[ncur] = sum;
    }

    cache_len = needed + 1;
    cache_cap = new_cap;
}

/* public function required by the judge */
seq_t a(idx_t n)
{
    ensure_size(n);
    return a_cache[n];
}
