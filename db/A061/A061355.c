/*
 * A061355  Denominator of Sum_{k=0..n} 1/k!.
 * https://oeis.org/A061355
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of sum_{k=0..n} 1/k!
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by iteratively adding 1/k! and reducing with gcd. */

#include <stdlib.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static size_t cache_cap = 0;
    static seq_t *den_cache = NULL;
    static idx_t max_computed = 0;

    /* static working variables */
    static seq_t cur_num = 1;   /* numerator of partial sum */
    static seq_t cur_den = 1;   /* denominator of partial sum */
    static seq_t fact   = 1;    /* current factorial value k! */

    /* initialise cache on first call */
    if (!den_cache) {
        cache_cap = 1;
        den_cache = (seq_t *)malloc(sizeof(seq_t));
        if (!den_cache) return 0;          /* allocation failed */
        den_cache[0] = 1;                  /* a(0) = 1 */
        max_computed = 0;
    }

    /* return cached value for already computed n */
    if (n <= max_computed) {
        return den_cache[n];
    }

    /* enlarge cache if needed */
    if (cache_cap <= (size_t)n) {
        size_t new_cap = n + 1;
        seq_t *newp = (seq_t *)realloc(den_cache, sizeof(seq_t) * new_cap);
        if (!newp) return 0;              /* allocation failed */
        den_cache = newp;
        cache_cap = new_cap;
    }

    /* iterative computation from max_computed+1 up to n */
    for (idx_t k = max_computed + 1; k <= n; ++k) {
        /* update factorial: fact = k! */
        fact = fact * (seq_t)k;

        /* add 1/k! to current fraction: (cur_num / cur_den) + 1/fact */
        seq_t g1 = gcd(cur_den, fact);         /* reduce cross product */
        seq_t den_div_g = cur_den / g1;
        seq_t fact_div_g = fact / g1;

        seq_t new_num = cur_num * fact_div_g + den_div_g;
        seq_t new_den = den_div_g * fact;

        seq_t g2 = gcd(new_num, new_den);      /* final reduction */
        cur_num = new_num / g2;
        cur_den = new_den / g2;

        den_cache[k] = cur_den;                /* store result */
    }

    max_computed = n;
    return cur_den;
}
