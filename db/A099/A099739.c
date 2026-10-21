/*
 * A099739  a(n) = Sum_{k=1..n} k!*mu(k), where mu() is the Moebius function.
 * https://oeis.org/A099739
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=1..n} k!*mu(k)
 * Range:     i8 n <= 5, i16 n <= 9, i32 n <= 12, i64 n <= 20, i128 n <= 33,
 *            big any n
 * Helpers:   mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes the sequence with memoisation; returns a(n) for all n ≥ 1. */
/*  The implementation uses a static cache to store previously computed */
#include <stddef.h>      /* size_t */
#include <stdlib.h>     /* malloc, realloc */
#include "../utils/mobius.h"  /* seq_t mobius(idx_t m) */

/*  Cached values:  sums[i]  =  a(i)   (1‑based),  sums[0] = 0.   */
static seq_t  *sums        = NULL;   /* cache array (1‑based) */
static size_t   cap         = 0;     /* array capacity (size+1)  */
static idx_t    max_computed = 0;    /* highest n for which sums are known */
static seq_t   fact          = 1;    /* factorial of max_computed */

/*  Public entry point                                                */
seq_t a(idx_t n)
{
    /*  Initialise cache on first call                                   */
    if (sums == NULL) {
        cap   = (size_t)n + 1;
        sums  = (seq_t *)malloc(cap * sizeof(seq_t));
        if (sums == NULL) return 0;                      /* allocation failed */
        sums[0] = 0;                                       /* a(0) unused          */
        max_computed = 0;                                  /* no terms computed yet*/
        fact = 1;                                          /* 0! = 1               */
    }

    /*  Grow the array if necessary                                     */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(sums, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0;                         /* allocation failed */
        sums = tmp;
        cap = new_cap;
    }

    /*  If the requested value has already been computed, return it   */
    if (n <= max_computed)
        return sums[n];

    /*  Compute missing terms from max_computed+1 up to n              */
    for (idx_t k = max_computed + 1; k <= n; ++k) {
        fact = fact * (seq_t)k;                    /* k! = (k-1)! * k   */
        sums[k] = sums[k - 1] + fact * mobius(k);  /* a(k) = a(k-1) + k!*mu(k) */
    }

    /*  Record the new maximum index                                       */
    max_computed = n;

    return sums[n];
}
