/*
 * A220035  Number of 5 X n arrays of the minimum value of corresponding
 *          elements and their horizontal or diagonal neighbors in a random,
 *          but sorted with lexicographically nondecreasing rows and
 *          nonincreasing columns, 0..1 5 X n array.
 * https://oeis.org/A220035
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) - 10*a(n-2) + 10*a(n-3) - 5*a(n-4) + a(n-5) for
 *            n>11 with initial values a(1)=6, a(2)=10, a(3)=21, a(4)=55,
 *            a(5)=111, a(6)=192. For 7≤n≤11 the sequence equals the empirical
 *            polynomial a(n) = (n⁴)/24 - (n³)/12 + (95 n²)/24 + (578 n)/24 -
 *            132 (i.e. 578/24 = 289/12)
 * Range:     u8 n <= 6, u16 n <= 35, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   term_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/term_small.h"

/* Compute the polynomial value for n ≥ 7 */
static inline seq_t poly_term(idx_t n)
{
    /* all calculations are done in unsigned 64‑bit to avoid overflow before the last
       division, then cast to seq_t for the final result. */
    uint64_t nn = (uint64_t)n;
    uint64_t num = nn*nn*nn*nn          /* n⁴ */
                - 2*nn*nn*nn           /* -2 n³  → - (1/12)n³ * 24 = -2 n³ */
                + 95*nn*nn             /* +95 n²  → + (95/24) n² * 24 = + 95 n² */
                + 578*nn               /* +578 n  → + (578/24) n * 24 = + 578 n  */
                - 3168;                /* -132*24 bottom part */
    return (seq_t)(num / 24);
}

seq_t a(idx_t n)
{
    if (n <= 6)
        return term_small(n);

    /* we need up to n terms in the cache (1‑based). */
    size_t need = (size_t)n;

    static seq_t *cache = NULL;
    static size_t cache_len = 0; /* number of terms currently stored */

    if (cache_len < need) {
        /* enlarge the cache */
        seq_t *tmp = (seq_t *)realloc(cache,
                                      need * sizeof(seq_t));
        if (!tmp)
            return 0; /* allocation failed */
        cache = tmp;

        /* initialise or extend the cache */
        size_t i;
        for (i = cache_len; i < need; ++i) {
            idx_t idx = (idx_t)(i + 1);       /* 1‑based index */
            if (idx <= 6) {
                cache[i] = term_small(idx);
            } else if (idx <= 11) {          /* use polynomial for 7‑11 */
                cache[i] = poly_term(idx);
            } else {                         /* recurrence for n>11 */
                /* a(n) = 5*a(n-1) - 10*a(n-2) + 10*a(n-3) - 5*a(n-4) + a(n-5) */
                seq_t t = 5 * cache[i - 1]   /* 5*a(n-1) */
                         - 10 * cache[i - 2] /* -10*a(n-2) */
                         + 10 * cache[i - 3] /* +10*a(n-3) */
                         - 5 * cache[i - 4]  /* -5*a(n-4) */
                         +   cache[i - 5];   /* +a(n-5) */
                cache[i] = t;
            }
        }
        cache_len = need;
    }

    return cache[need - 1];
}
