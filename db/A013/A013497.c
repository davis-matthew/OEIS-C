/*
 * A013497  Expansion of e.g.f. log(cosh(x)-log(x+1)).
 * https://oeis.org/A013497
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n! * coefficient of x^n in log(cosh(x) - log(1+x))
 * Range:     i8 n <= 6, i16 n <= 9, i32 n <= 14, i64 n <= 21, i128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A013497: Expansion of e.g.f. log(cosh(x)-log(x+1)) */
/* The function computes the sequence term by term using the derived
   recurrence:  a(1) = -1,
   a(m) = f(m) - sum_{k=1}^{m-1} a(m-k) * f(k) * C(m-1,k),
   where f(k) = (k-1)! + 1  if k even,  f(k) = -(k-1)!  if k odd,
   and C(n,k) is the binomial coefficient.  All arithmetic is done
   with the type seq_t. */

#include <stdlib.h>   /* malloc, realloc, free */
#include <stdbool.h>  /* bool, false, true */

static seq_t *a_cache = NULL;      /* Cached a(n) values */
static seq_t *f_cache = NULL;      /* Cached f(n) values   */
static idx_t cache_n = 0;          /* Highest n for which values are computed */

/* Ensure that the caches contain values up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (n <= cache_n)
        return;

    /* Allocate new memory for the required number of elements. */
    size_t new_sz = (size_t)(n + 1);
    seq_t *a_new = NULL;
    seq_t *f_new = NULL;

    a_new = (seq_t *)realloc(a_cache, new_sz * sizeof(seq_t));
    f_new = (seq_t *)realloc(f_cache, new_sz * sizeof(seq_t));

    if (!a_new || !f_new) {
        /* Allocation failure: terminate the program. */
        abort();
    }
    a_cache = a_new;
    f_cache = f_new;

    /* Compute f(k) for all k from 1 to n.  f(0) is unused but kept as 1. */
    seq_t fac = 1;            /* fac = (k-1)! */
    f_cache[0] = 1;           /* by definition */
    for (idx_t k = 1; k <= n; ++k) {
        if ((k & 1) == 0) {   /* k even */
            f_cache[k] = fac + (seq_t)1;
        } else {              /* k odd */
            f_cache[k] = -fac;
        }
        fac *= (seq_t)k;      /* update fac to k! for next iteration */
    }

    /* Initialize a(1) if it hasn't been set yet. */
    if (cache_n == 0) {
        a_cache[1] = f_cache[1];     /* a(1) = f(1) = -1 */
        cache_n = 1;
    }

    /* Compute a(m) for m = cache_n+1 .. n using the recurrence. */
    for (idx_t m = cache_n + 1; m <= n; ++m) {
        seq_t sum = 0;
        seq_t binom = 1;           /* binom(m-1,0) */
        for (idx_t j = 1; j < m; ++j) {
            /* Update binomial coefficient: C(m-1, j) = C(m-1, j-1) * (m-j)/j */
            binom = binom * (seq_t)(m - j);
            binom = binom / (seq_t)j;
            seq_t term = a_cache[m - j] * f_cache[j];
            term = term * binom;
            sum += term;
        }
        a_cache[m] = f_cache[m] - sum;
    }

    cache_n = n;
}

/* External linkage function as required. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* undefined for n=0; return 0 to satisfy function signature */

    ensure_cache(n);
    return a_cache[n];
}
