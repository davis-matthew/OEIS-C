/*
 * A063368  Dimension of the space of weight n cuspidal newforms for Gamma_1(
 *          95 ).
 * https://oeis.org/A063368
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = a(n-4) + a(n-6) - a(n-10) for n > 13, * with initial
 *            terms: * a(2) = -1, a(3) = 275, a(4) = 606, a(5) = 934, * a(6) =
 *            1262, a(7) = 1590, a(8) = 1918, a(9) = 2246, * a(10) = 2574,
 *            a(11) = 2902, a(12) = 3230, a(13) = 3562
 * Range:     i8 n <= 2, i16 n <= 39, i32 n <= 39, i64 n <= 39, i128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache   = NULL;               /* array storing a(2),a(3),... */
static idx_t  max_sc  = 1;                  /* highest n stored in cache */
static size_t sc_cap  = 0;                  /* allocated slots in cache */

/* initialise cache with the 12 base values a(2)..a(13) */
static void init_cache(void) {
    /* base values a(2) through a(13) */
    const seq_t base[12] = {
        -1,   275,   606,   934,  1262,
        1590, 1918, 2246, 2574, 2902,
        3230, 3562
    };
    size_t need_cap = 12;                  /* store indices 2..13 -> 12 elements */
    cache = (seq_t *)malloc(need_cap * sizeof(seq_t));
    sc_cap = need_cap;
    for (idx_t i = 2; i <= 13; ++i) {
        cache[(size_t)(i-2)] = base[i-2];
    }
    max_sc = 13;
}

/* make sure that cache covers all n up to requested n */
static void ensure_cache(idx_t n) {
    if (cache == NULL)
        init_cache();
    if (n <= max_sc)
        return;

    /* enlarge cache if necessary */
    size_t needed_cap = (size_t)n - 1;          /* n-1 elements for indices 2..n */
    if (needed_cap > sc_cap) {
        cache = (seq_t *)realloc(cache, needed_cap * sizeof(seq_t));
        sc_cap = needed_cap;
    }

    /* compute missing terms using the recurrence */
    for (idx_t i = max_sc + 1; i <= n; ++i) {
        size_t ia = (size_t)(i - 4 - 2);
        size_t ib = (size_t)(i - 6 - 2);
        size_t ic = (size_t)(i - 10 - 2);
        seq_t val = cache[ia] + cache[ib] - cache[ic];
        cache[(size_t)(i-2)] = val;
    }
    max_sc = n;
}

seq_t a(idx_t n) {
    if (n < 2)                     /* undefined for indices below the offset */
        return 0;
    ensure_cache(n);
    return cache[(size_t)(n-2)];
}
