/*
 * A167680  Number of reduced words of length n in Coxeter group on 20
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^15 = I.
 * https://oeis.org/A167680
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) - 18*(a(n-1)+...+a(n-14)) + 171*a(n-15) = b(n)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where b(0)=1, b(1..14)=2, b(15)=1, b(n>15)=0.  This can be     */
/* rearranged to compute the required terms iteratively.             */

#include <stdlib.h>

/* Static cache of computed terms */
static seq_t *cache = NULL;          /* array of a(i) values           */
static size_t cache_cap = 0;         /* currently allocated size       */
static size_t computed_upto = (size_t)-1; /* highest index computed */

/* Ensure that cache has space for indices 0..n */
static void ensure_cache(idx_t n) {
    size_t need = (size_t)(n + 1);
    if (need <= cache_cap) return;

    seq_t *new_cache = (seq_t *)realloc(cache, need * sizeof(seq_t));
    if (!new_cache) abort();          /* out of memory */
    cache = new_cache;
    for (size_t i = cache_cap; i < need; ++i) cache[i] = 0;
    cache_cap = need;
}

/* Compute all missing terms up to (and including) target n */
static void compute_up_to(idx_t target) {
    size_t len = (size_t)(target + 1);
    ensure_cache(target);

    size_t start = (computed_upto == (size_t)-1) ? 0 : computed_upto + 1;

    for (size_t i = start; i < len; ++i) {
        seq_t sum = 0;
        for (int k = 1; k <= 14 && i >= (size_t)k; ++k)
            sum += cache[i - k];
        seq_t term = sum * (seq_t)18;
        if (i >= 15) term -= cache[i - 15] * (seq_t)171;

        /* RHS value b(i) */
        seq_t rhs = 0;
        if (i == 0 || i == 15) rhs = (seq_t)1;
        else if (i >= 1 && i <= 14) rhs = (seq_t)2;

        term += rhs;
        cache[i] = term;
    }
    computed_upto = (size_t)target;
}

seq_t a(idx_t n) {
    if ((idx_t)-1 == n) return (seq_t)0;     /* safety on unsigned underflow */
    compute_up_to(n);
    return cache[(size_t)n];
}
