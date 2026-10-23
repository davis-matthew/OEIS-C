/*
 * A170720  Number of reduced words of length n in Coxeter group on 39
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^50 = I.
 * https://oeis.org/A170720
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A170720: number of reduced words of length n in the Coxeter
   group on 39 generators with relations (S_i)^2 = (S_i S_j)^50 = I.
   The generating function is
       G(t) = (t^50 + 2 t^49 + … + 2 t + 1) /
              (703 t^50 - 37 t^49 – … – 37 t + 1)
   From Q(t)*G(t)=P(t) we obtain the linear recurrence
       a(n) = p(n) + 37·S(n-1) – 703·a(n-50)
   where
       p(n) = { 1, if n = 0 or 50
                2, if 1 ≤ n ≤ 49
                0, otherwise }
   and
       S(k) = a(k) + a(k-1) + … + a(k-48) (sum of the last 49 terms,
       with fewer terms when k < 48).
   The recurrence is initialized with a(0)=1. */

#include <stdlib.h>
#include <stddef.h>

static seq_t *a_cache = NULL;         /* cache of computed terms */
static size_t cache_len = 0;          /* number of entries in a_cache */
static seq_t sum_last49 = 0;          /* sum of the last up to 49 terms */
static idx_t last_idx = 0;            /* last index that has been computed */

seq_t a(idx_t n)
{
    /* allocate/extend cache if necessary */
    if (cache_len == 0) {
        a_cache = (seq_t *)realloc(NULL, sizeof(seq_t));
        if (!a_cache) return 0;          /* out of memory; not expected */
        a_cache[0] = 1;                  /* a(0) = 1 */
        cache_len = 1;
        sum_last49 = 1;                 /* after a(0), sum of last terms is a(0) */
        last_idx = 0;
    }

    /* compute until we reach n */
    while ((idx_t)(cache_len - 1) < n) {
        idx_t i = cache_len;            /* next index to compute */

        /* coefficient p(i) from numerator */
        seq_t p;
        if (i == 0 || i == 50)
            p = 1;
        else if (i >= 1 && i <= 49)
            p = 2;
        else
            p = 0;

        /* a(i) = p + 37*sum_last49 – 703*a(i-50) (if i ≥ 50) */
        seq_t term = p + 37 * sum_last49;
        if (i >= 50)
            term -= 703 * a_cache[i - 50];

        /* store the value */
        a_cache = (seq_t *)realloc(a_cache, (cache_len + 1) * sizeof(seq_t));
        if (!a_cache) return 0;          /* out of memory; not expected */
        a_cache[i] = term;
        cache_len++;

        /* update sum_last49 for next iteration */
        sum_last49 += term;
        if (i >= 49)                     /* window of size 49 reached */
            sum_last49 -= a_cache[i - 49];
    }

    last_idx = n;
    return a_cache[n];
}
