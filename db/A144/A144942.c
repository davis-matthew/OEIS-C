/*
 * A144942  Expansion of x^2*(3*x^3+145*x^2-507*x-25) /
 *          ((x-1)*(x^2-34*x+1)*(x^2+34*x+1)).
 * https://oeis.org/A144942
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) satisfies the linear recurrence a(n) = a(n-1) + 1154*a(n-2)
 *            - 1154*a(n-3) - a(n-4) + a(n-5) for n ≥ 6, with initial values
 *            a(1) = 0, a(2) = 25, a(3) = 532, a(4) = 29237, a(5) = 614312.
 *            Offset: 1 (a(1) is the first term)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;     /* array of stored terms, cache[0] unused */
static idx_t cached_max = 0;    /* largest n for which cache[n] is valid */

seq_t a(idx_t n)
{
    /* handle small n directly (offset 1) */
    if (n <= 0) return 0;   /* undefined; return 0 for safety */

    /* allocate initial cache up to 5 if first call */
    if (!cache) {
        size_t init_cap = 6;          /* indices 0 .. 5 */
        cache = (seq_t*)malloc(sizeof(seq_t) * init_cap);
        cache[0] = 0;                /* unused */
        cache[1] = 0;
        cache[2] = 25;
        cache[3] = 532;
        cache[4] = 29237;
        cache[5] = 614312;
        cached_max = 5;
    }

    /* extend cache if needed */
    if (n > (idx_t)(cached_max)) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp) {
            /* allocation failed; abort silently */
            return 0;
        }
        cache = tmp;
        /* compute new terms */
        for (idx_t i = (idx_t)(cached_max + 1); i <= n; ++i) {
            seq_t term_before = cache[i - 1]
                               + ((seq_t)1154 * cache[i - 2] + cache[i - 5]);
            seq_t term_after  = ((seq_t)1154 * cache[i - 3] + cache[i - 4]);
            cache[i] = term_before - term_after;
        }
        cached_max = (idx_t)n;
    }

    return cache[n];
}
