/*
 * A170051  Number of reduced words of length n in Coxeter group on 42
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^36 = I.
 * https://oeis.org/A170051
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 40·∑_{i=1}^{35}a(n−i) − 820·a(n−36) + N(n) where N(0)=1,
 *            N(n)=2 for 1≤n≤35, N(36)=1, and N(n)=0 otherwise. The recurrence
 *            is homogeneous for n≥36
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* seq_t and idx_t are supplied by the build system.  */
typedef seq_t  seq_t;
typedef idx_t  idx_t;

/* Computes the nth term of A170051.  */
seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;     /* array of computed terms   */
    static idx_t  computed = 0;       /* largest index computed   */
    static seq_t sumPrev35;           /* Σ_{i=1}^{35} a(cur−i) at step cur */

    /* --------- 1st term is trivial  --------- */
    if (n == 0)
        return (seq_t)1;

    /* -------- 1st allocation  -------- */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* allocation error          */
        cache[0] = (seq_t)1;                  /* a(0)                      */
        computed = 0;
        sumPrev35 = cache[0];                 /* for cur = 1              */
    }

    /* -------- If n is already known -------- */
    if (n <= computed)
        return cache[n];

    /* -------- Grow the cache if necessary -------- */
    size_t old_cap = computed + 1;
    size_t new_cap = old_cap;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;                        /* exponential growth       */

    if (new_cap != old_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;            /* realloc failure          */
        cache = tmp;
    }

    /* -------- Generate terms up to n -------- */
    for (idx_t cur = computed + 1; cur <= n; ++cur) {
        /*  Σ a(cur-i) for i=1..35  */
        seq_t term = sumPrev35 * (seq_t)40;

        /*  subtract 820·a(cur-36) if cur≥36  */
        if (cur >= 36)
            term -= (cache[cur - 36] * (seq_t)820);

        /*  add the numerator contribution N(cur)  */
        if (cur == 0 || cur == 36)
            term += (seq_t)1;
        else if (cur >= 1 && cur <= 35)
            term += (seq_t)2;

        /*  store the new term  */
        cache[cur] = term;

        /*  update rolling sum for the next iteration  */
        sumPrev35 += term;
        if (cur >= 35)                       /* drop the 35‑th old term */
            sumPrev35 -= cache[cur - 35];
    }

    computed = n;
    return cache[n];
}
