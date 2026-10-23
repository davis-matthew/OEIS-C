/*
 * A151882  Let g be a permutation of [1..n] having say j_i cycles of length
 *          i, with Sum_i i*j_i = n; sequence gives Sum_g Sum_i (j_i)^2.
 * https://oeis.org/A151882
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * ( H_n + ∑_{i=1}^{⌊n/2⌋} 1/i^2 ) equivalent
 *            recurrence: a(0)=0 ; a(n)=n*a(n-1)+(n-1)!+(n even?4*(n-1)!/n:0)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Only the required headers are included; seq_t and idx_t are provided in the build environment.  */
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed values for all smaller n */
    static seq_t *val = NULL;          /* array a[0..computed]          */
    static size_t cap = 0;              /* capacity of allocated array    */
    static idx_t computed = 0;          /* largest n processed so far     */
    static seq_t fac = (seq_t)1;        /* current factorial, initially 0! */

    /* initialise the cache on the first call */
    if (cap == 0) {
        cap = 16;
        val = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!val) return (seq_t)0;   /* fallback on allocation failure */
        val[0] = (seq_t)0;           /* a(0)=0, sentry value          */
    }

    /* extend the precomputation up to n if necessary */
    while (computed < n) {
        idx_t k = computed + 1;      /* new index to compute (k ≥ 1)  */

        /* factorial update: fac becomes k!  */
        fac = fac * (seq_t)k;                 /* unsigned multiplication */

        /* (k-1)! is fac / k (exact division)  */
        seq_t oldfac = fac / (seq_t)k;

        /* term from (k-1)!  */
        seq_t add = oldfac;

        /* extra term when k is even  */
        if ((k & 1) == 0) {
            seq_t extra = oldfac * (seq_t)4 / (seq_t)k;   /* exact */
            add += extra;
        }

        /* recurrence: a(k) = k*a(k-1) + add */
        seq_t value = (seq_t)k * val[computed] + add;

        /* enlarge the cache if needed */
        if ((size_t)(k + 1) > cap) {
            size_t newcap = cap * 2;
            seq_t *tmp = (seq_t *)realloc(val, newcap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;  /* fallback on allocation failure */
            val = tmp;
            cap = newcap;
        }

        /* store the new value */
        val[k] = value;

        /* update the global state */
        computed = k;
    }

    return val[n];
}
