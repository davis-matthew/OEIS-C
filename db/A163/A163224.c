/*
 * A163224  Number of reduced words of length n in Coxeter group on 41
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^4 = I.
 * https://oeis.org/A163224
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 39*a(n-1) + 39*a(n-2) + 39*a(n-3) – 780*a(n-4) initial
 *            values: a(0)=1, a(1)=41, a(2)=1640, a(3)=65600, a(4)=2623180
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* memoised values */
    static idx_t capacity = 0;    /* allocated array size          */
    static idx_t valid = 0;       /* number of values currently computed */

    /* Initialise with known starting terms (done once) */
    if (cache == NULL) {
        capacity = 5;                      /* at least indices 0..4     */
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (cache == NULL) return 0;        /* allocation failed         */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)41;
        cache[2] = (seq_t)1640;
        cache[3] = (seq_t)65600;
        cache[4] = (seq_t)2623180;
        valid = 5;                         /* indices 0..4 are known   */
    }

    /* If the requested index is already known, return it immediately */
    if (n < valid)
        return cache[n];

    /* Grow the cache if necessary */
    if (n + 1 > capacity) {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0;         /* reallocation failed     */
        cache = tmp;
        capacity = new_cap;
    }

    /* Compute missing values using the recurrence */
    for (idx_t i = valid; i <= n; ++i) {
        seq_t term =
            ((seq_t)39) * cache[i - 1] +
            ((seq_t)39) * cache[i - 2] +
            ((seq_t)39) * cache[i - 3] -
            ((seq_t)780) * cache[i - 4];
        cache[i] = term;
    }
    valid = n + 1;   /* all indices up to n are now valid */

    return cache[n];
}
