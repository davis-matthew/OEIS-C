/*
 * A049027  G.f.: (1-2*x*c(x))/(1-3*x*c(x)) where c(x) = (1 -
 *          sqrt(1-4*x))/(2*x) is the g.f. for Catalan numbers A000108.
 * https://oeis.org/A049027
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 30, u128 n <= 59,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A049027
 *
 * Formula:
 *   a(0) = 1,
 *   a(1) = 1,
 *   a(n) = (9*a(n-1) - Catalan(n-1)) / 2   for n >= 2
 *
 * The recurrence follows from the generating function
 *   G(x) = (1-2*x*c(x))/(1-3*x*c(x)),  c(x)=Catalan(g.f.).
 * Values are memoised so that calls in any order remain fast.
 */

#include <stddef.h>      /* for size_t */
#include <stdlib.h>      /* for malloc, realloc, free */
#include "../utils/catalan.h"   /* catalan(idx_t) */

seq_t a(idx_t n)
{
    /* --------- static memoisation table --------- */
    static seq_t *cache   = NULL;    /* array of computed a(i) */
    static size_t cache_cap = 0;      /* allocated capacity */
    static idx_t  max_comp = 0;       /* highest index computed (>=1) */

    /* ---------- initialisation (once) ---------- */
    if (cache == NULL) {
        /* allocate space for a(0) and a(1) */
        cache = (seq_t *)malloc(sizeof(seq_t) * 2);
        if (!cache) return 0;                 /* allocation failure */
        cache_cap = 2;
        cache[0] = (seq_t)1;                  /* a(0) */
        cache[1] = (seq_t)1;                  /* a(1) */
        max_comp = 1;                         /* we have values up to 1 */
    }

    /* ---------- handle trivial requests ---------- */
    if (n <= max_comp) return cache[n];

    /* ---------- ensure array large enough ---------- */
    size_t needed = (size_t)n + 1;
    if (needed > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap < needed) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                 /* allocation failure */
        cache   = tmp;
        cache_cap = new_cap;
    }

    /* ---------- compute missing terms ---------- */
    for (idx_t i = max_comp + 1; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        seq_t cat  = catalan(i - 1);
        seq_t next = ((seq_t)9 * prev - cat) / (seq_t)2;
        cache[i] = next;
    }
    max_comp = n;

    return cache[n];
}
