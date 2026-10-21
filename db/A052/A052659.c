/*
 * A052659  Expansion of e.g.f. (1-2x)(1-x)/(1-4x+2x^2).
 * https://oeis.org/A052659
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence formula from OEIS A052659:
   a(0) = 1, a(1) = 1, a(2) = 8
   For n ≥ 1:
     a(n+2) = (4n + 8) * a(n+1) - (2n^2 + 6n + 4) * a(n)
   (Indices are the same as the OEIS offset 0.) */
#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for memoisation, indices 0..max_n inclusive */
    static seq_t *cache = NULL;
    static idx_t max_n = 0;   /* highest n computed */
    static idx_t capacity = 0; /* allocated size of cache */

    /* direct return for initial terms */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)8;

    /* initialise cache if first use */
    if (!cache) {
        capacity = 4;                      /* enough for indices 0..3 */
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!cache) return 0;              /* allocation failed, return 0 */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)8;
        max_n = 2;
    }

    /* compute missing values up to requested n */
    if (n > max_n) {
        for (idx_t i = max_n + 1; i <= n; ++i) {
            idx_t k = i - 2;                      /* recurrence uses a(k) and a(k+1) */
            seq_t term1 = ((seq_t)(4 * k + 8)) * cache[k + 1];
            seq_t term2 = ((seq_t)(2 * k * k + 6 * k + 4)) * cache[k];
            seq_t val = term1 - term2;                 /* a(i) */

            /* grow cache if necessary */
            if (i >= capacity) {
                idx_t new_cap = capacity * 2;
                if (new_cap <= i) new_cap = i + 1;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return 0;          /* allocation failed */
                cache = tmp;
                capacity = new_cap;
            }
            cache[i] = val;
        }
        max_n = n;
    }

    return cache[n];
}
