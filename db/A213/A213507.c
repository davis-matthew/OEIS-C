/*
 * A213507  E.g.f.: exp( Sum_{n>=1} A000108(n)*x^n/n ), where A000108(n) =
 *          binomial(2*n,n)/(n+1) forms the Catalan numbers.
 * https://oeis.org/A213507
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A213507: a(n) defined by the EGF
   A(x) = exp( sum_{k>=1} Catalan(k) * x^k / k )
   Recurrence:
   a(0) = 1
   a(n) = sum_{t=0}^{n-1} a(n-1-t) * Catalan(t+1) *
          (n-1)! / (n-1-t)!   for n >= 1
   The implementation uses an incremental cache for a(n) and for
   Catalan numbers, and evaluates the above recurrence exactly in
   the seq_t type. */

/* standard includes */
#include <stdlib.h>

/* OEIS utility: Catalan numbers (Catalan(k) = binom(2k,k)/(k+1) ) */
#include "../utils/catalan.h"

static seq_t *a_cache = NULL;     /* a(n) values  */
static seq_t *cat_cache = NULL;   /* Catalan(k) values  */
static idx_t  a_max   = (idx_t)-1;/* highest n computed in a_cache  */
static idx_t  cat_max = 0;        /* highest k computed in cat_cache */

/* Ensure that cat_cache contains Catalan(k) for all k <= n */
static void
ensure_catalan_upto(idx_t n)
{
    if (n < cat_max)
        return;
    /* resize cat_cache if needed */
    cat_cache = (seq_t *)realloc(cat_cache, (n + 1) * sizeof(seq_t));
    if (!cat_cache) abort();
    /* compute missing values */
    for (idx_t k = cat_max; k <= n; ++k)
        cat_cache[k] = catalan(k);
    cat_max = n + 1;
}

/* Ensure that a_cache contains a(n) for all n <= target */
static void
ensure_a_upto(idx_t target)
{
    if (a_cache && a_max >= target)
        return;
    /* resize a_cache if needed */
    a_cache = (seq_t *)realloc(a_cache, (target + 1) * sizeof(seq_t));
    if (!a_cache) abort();

    /* initialise base case if cache empty */
    if (a_max == (idx_t)-1) {
        a_cache[0] = (seq_t)1;
        a_max = 0;
    }

    /* growth loop: compute missing a(n) values */
    for (idx_t n = a_max + 1; n <= target; ++n) {
        /* ensure needed Catalan numbers are ready */
        ensure_catalan_upto(n);             /* need up to Catalan(n) */

        seq_t sum = 0;
        seq_t f   = 1;                       /* (n-1)!/(n-1-0)! */
        for (idx_t t = 0; t < n; ++t) {
            seq_t term = a_cache[n - 1 - t] *
                         cat_cache[t + 1] *   /* Catalan(t+1) */
                         f;
            sum += term;
            if (t + 1 < n)
                f *= (seq_t)(n - 1 - t);
        }
        a_cache[n] = sum;
    }
    a_max = target;
}

seq_t a(idx_t n)
{
    ensure_a_upto(n);
    return a_cache[n];
}
