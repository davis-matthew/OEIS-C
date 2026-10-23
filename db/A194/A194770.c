/*
 * A194770  E.g.f. 2*sqrt(3)/3*arctan(sqrt(3)*x/(x+2)).
 * https://oeis.org/A194770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 6, i16 n <= 9, i32 n <= 13, i64 n <= 21, i128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A194770
   Formula: a(3*n)   = 0,
           a(3*n+1) = (3*n)!,
           a(3*n+2) = -(3*n+1)!   (n >= 0)
   Recurrence (for documentation): a(n)+(n-1)a(n-1)+(n-1)(n-2)a(n-2)=0
   Implementation uses incremental factorials of multiples of three. */

#include <stdlib.h>

/* cache of factorials (3*k)! for k = 0,1,2,... */
static seq_t *fact_cache = NULL;   /* dynamically allocated array */
static idx_t  fact_cap   = 0;      /* current allocated size */
static idx_t  fact_len   = 0;      /* number of factorials computed */

/* Ensure that fact_cache contains factorials up to index "need" (inclusive) */
static void ensure_factorial(idx_t need)
{
    if (need < fact_len) return;            /* already computed */

    /* extend allocation if necessary */
    if (fact_cap <= need) {
        idx_t new_cap = (fact_cap > 0) ? fact_cap * 2 : 8;
        if (new_cap < need + 1) new_cap = need + 1;
        seq_t *tmp = (seq_t*)realloc(fact_cache, new_cap * sizeof(seq_t));
        fact_cache = tmp;
        fact_cap   = new_cap;
    }

    /* compute missing factorials */
    for (idx_t k = fact_len; k <= need; ++k) {
        if (k == 0) {
            fact_cache[0] = (seq_t)1;          /* (0)! = 1 */
        } else {
            seq_t val   = fact_cache[k-1];
            idx_t m1    = 3*k;                 /* multiplies to get (3*k)! */
            idx_t m2    = m1 - 1;
            idx_t m3    = m1 - 2;
            val *= (seq_t)m1;
            val *= (seq_t)m2;
            val *= (seq_t)m3;
            fact_cache[k] = val;
        }
    }
    fact_len = need + 1;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;              /* offset is 1 */

    idx_t mod = n % 3u;
    if (mod == 0) {
        return (seq_t)0;                      /* a(3*k) = 0 */
    } else if (mod == 1) {
        idx_t k = (n - 1) / 3;                /* compute (3*k)! */
        ensure_factorial(k);
        return fact_cache[k];
    } else { /* mod == 2 */
        idx_t k   = (n - 2) / 3;              /* need (3*k+1)! = (3*k)! * (3*k+1) */
        ensure_factorial(k);
        seq_t  res = fact_cache[k];
        idx_t  extra = 3*k + 1;
        res *= (seq_t)extra;
        return -res;                          /* negative sign */
    }
}
