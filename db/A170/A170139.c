/*
 * A170139  Number of reduced words of length n in Coxeter group on 34
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^38 = I.
 * https://oeis.org/A170139
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A170139: Reduced words in a Coxeter group with 34 generators
   and relations S_i^2 = (S_i S_j)^38 = I.  The generating function is
   N(t)/D(t) with
        N(t) = 1 + 2t + 2t^2 + … + 2t^37 + t^38
        D(t) = 1 – 32t – 32t^2 – … – 32t^37 + 528t^38 .
   Hence the recurrence
        a(n) = Σ_{i=1}^{37} 32·a(n-i) – 528·a(n-38) + p(n)
   where   p(0)=1,  p(1…37)=2,  p(38)=1,  p(n)=0 for n>38,
   and a(0)=1. */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    size_t i;

    /* enlarge cache up to n+1 elements */
    if (n + 1 > cap) {
        size_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* allocation failed – abort */
            exit(EXIT_FAILURE);
        }
        cache = tmp;
        /* initialise new slots to zero (not really needed) */
        for (i = cap; i < new_cap; ++i) cache[i] = 0;
        cap = new_cap;
    }

    /* if value already computed, return it */
    if (n < cap && cache[n] != 0) { /* cache contains zero only for uncomputed values */
        return cache[n];
    }

    /* compute from offest of current cache to n */
    for (i = 0; i <= n; ++i) {
        if (i < cap && cache[i] != 0) continue; /* already computed */

        seq_t sum = 0;

        /* add p(i) term */
        if (i == 0)      sum = 1;
        else if (i <= 37) sum = 2;
        else if (i == 38) sum = 1;
        /* for i > 38, p(i) = 0, nothing to add */

        /* add 32 * a(i-j) for j = 1..37 */
        if (i >= 1) {
            seq_t t = cache[i-1] * ((seq_t)32);
            sum += t;
        }
        if (i >= 2) {
            seq_t t = cache[i-2] * ((seq_t)32);
            sum += t;
        }
        if (i >= 3) {
            seq_t t = cache[i-3] * ((seq_t)32);
            sum += t;
        }
        if (i >= 4) {
            seq_t t = cache[i-4] * ((seq_t)32);
            sum += t;
        }
        if (i >= 5) {
            seq_t t = cache[i-5] * ((seq_t)32);
            sum += t;
        }
        if (i >= 6) {
            seq_t t = cache[i-6] * ((seq_t)32);
            sum += t;
        }
        if (i >= 7) {
            seq_t t = cache[i-7] * ((seq_t)32);
            sum += t;
        }
        if (i >= 8) {
            seq_t t = cache[i-8] * ((seq_t)32);
            sum += t;
        }
        if (i >= 9) {
            seq_t t = cache[i-9] * ((seq_t)32);
            sum += t;
        }
        if (i >=10) {
            seq_t t = cache[i-10] * ((seq_t)32);
            sum += t;
        }
        if (i >=11) {
            seq_t t = cache[i-11] * ((seq_t)32);
            sum += t;
        }
        if (i >=12) {
            seq_t t = cache[i-12] * ((seq_t)32);
            sum += t;
        }
        if (i >=13) {
            seq_t t = cache[i-13] * ((seq_t)32);
            sum += t;
        }
        if (i >=14) {
            seq_t t = cache[i-14] * ((seq_t)32);
            sum += t;
        }
        if (i >=15) {
            seq_t t = cache[i-15] * ((seq_t)32);
            sum += t;
        }
        if (i >=16) {
            seq_t t = cache[i-16] * ((seq_t)32);
            sum += t;
        }
        if (i >=17) {
            seq_t t = cache[i-17] * ((seq_t)32);
            sum += t;
        }
        if (i >=18) {
            seq_t t = cache[i-18] * ((seq_t)32);
            sum += t;
        }
        if (i >=19) {
            seq_t t = cache[i-19] * ((seq_t)32);
            sum += t;
        }
        if (i >=20) {
            seq_t t = cache[i-20] * ((seq_t)32);
            sum += t;
        }
        if (i >=21) {
            seq_t t = cache[i-21] * ((seq_t)32);
            sum += t;
        }
        if (i >=22) {
            seq_t t = cache[i-22] * ((seq_t)32);
            sum += t;
        }
        if (i >=23) {
            seq_t t = cache[i-23] * ((seq_t)32);
            sum += t;
        }
        if (i >=24) {
            seq_t t = cache[i-24] * ((seq_t)32);
            sum += t;
        }
        if (i >=25) {
            seq_t t = cache[i-25] * ((seq_t)32);
            sum += t;
        }
        if (i >=26) {
            seq_t t = cache[i-26] * ((seq_t)32);
            sum += t;
        }
        if (i >=27) {
            seq_t t = cache[i-27] * ((seq_t)32);
            sum += t;
        }
        if (i >=28) {
            seq_t t = cache[i-28] * ((seq_t)32);
            sum += t;
        }
        if (i >=29) {
            seq_t t = cache[i-29] * ((seq_t)32);
            sum += t;
        }
        if (i >=30) {
            seq_t t = cache[i-30] * ((seq_t)32);
            sum += t;
        }
        if (i >=31) {
            seq_t t = cache[i-31] * ((seq_t)32);
            sum += t;
        }
        if (i >=32) {
            seq_t t = cache[i-32] * ((seq_t)32);
            sum += t;
        }
        if (i >=33) {
            seq_t t = cache[i-33] * ((seq_t)32);
            sum += t;
        }
        if (i >=34) {
            seq_t t = cache[i-34] * ((seq_t)32);
            sum += t;
        }
        if (i >=35) {
            seq_t t = cache[i-35] * ((seq_t)32);
            sum += t;
        }
        if (i >=36) {
            seq_t t = cache[i-36] * ((seq_t)32);
            sum += t;
        }
        if (i >=37) {
            seq_t t = cache[i-37] * ((seq_t)32);
            sum += t;
        }

        /* subtract 528 * a(i-38) if i >= 38 */
        if (i >= 38) {
            seq_t t = cache[i-38] * ((seq_t)528);
            sum -= t;
        }

        cache[i] = sum;
    }

    return cache[n];
}
