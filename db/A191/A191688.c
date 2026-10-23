/*
 * A191688  E.g.f. 1/(1-sin(x)^3).
 * https://oeis.org/A191688
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 12, i64 n <= 20, i128 n <= 20,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A191688: E.g.f. 1/(1 - sin(x)^3)
   Recurrence: 
      a(0) = 1
      a(n) = Σ_{m odd 1≤m≤n} binom(n,m) * s(m) * a(n-m)
   where  s(m) = ((-1)^((m-1)/2) * (3 - 3^m)) / 4   (m odd)

   Implements caching of the a(n) sequence.  The recurrence uses only
   integer arithmetic on seq_t and binom().
*/

#include <stdlib.h>
#include "../utils/binom.h"

static seq_t *a_cache   = NULL;  /* cache of a(n) values          */
static seq_t *s_cache   = NULL;  /* cache of s(m) coefficients   */
static seq_t *pow3_cache = NULL; /* cache of 3^m for odd m       */
static idx_t   cache_size   = 0; /* current allocated size       */
static idx_t   pow3_size    = 0; /* size of pow3_cache           */

/* Ensure caches are large enough for indices up to n. */
static void ensure_cache(idx_t n)
{
    if (n < cache_size)
        return;                   /* already have data */

    idx_t old_size = cache_size;
    idx_t new_size = n + 1;

    /* Reallocate caches */
    a_cache     = (seq_t *)realloc(a_cache,     new_size * sizeof(seq_t));
    s_cache     = (seq_t *)realloc(s_cache,     new_size * sizeof(seq_t));
    pow3_cache  = (seq_t *)realloc(pow3_cache,  new_size * sizeof(seq_t));

    /* Initialise new entries */
    if (old_size == 0)
        a_cache[0] = (seq_t)1;    /* a(0) = 1 */

    /* Compute s_cache and pow3_cache for indices from old_size to n */
    for (idx_t i = old_size; i <= n; ++i) {
        if (i & 1U) {                 /* odd i: compute s(i) */
            /* 3^i for odd i.  Use pow3_cache of previous odd index. */
            if (i == 1U)
                pow3_cache[i] = (seq_t)3;          /* 3^1 */
            else
                pow3_cache[i] = pow3_cache[i - 2] * (seq_t)9; /* 3^(i) = 3^(i-2) * 9 */

            seq_t sign = (((i - 1U) >> 1) & 1U) ? (seq_t)-1 : (seq_t)1;
            seq_t tmp  = (seq_t)3 - pow3_cache[i];      /* 3 - 3^i */
            tmp        = tmp / (seq_t)4;
            s_cache[i] = sign * tmp;
        } else {                      /* even i: s(i) = 0 */
            s_cache[i] = (seq_t)0;
        }
    }

    /* Compute a(i) for i from old_size to n */
    for (idx_t i = old_size; i <= n; ++i) {
        if (i == 0U)
            continue;                     /* already set to 1 */

        seq_t sum = (seq_t)0;
        for (idx_t m = 1U; m <= i; m += 2U) {  /* odd m only */
            seq_t term = s_cache[m] * a_cache[i - m];
            term = term * binom(i, m);         /* binom(i, m) is seq_t */
            sum += term;
        }
        a_cache[i] = sum;
    }

    cache_size = new_size;
}

/* Public API: returns a(n) for any n >= 0, any order of calls. */
seq_t a(idx_t n)
{
    if (n == 0U)
        return (seq_t)1;          /* a(0) = 1 */

    ensure_cache(n);
    return a_cache[n];
}
