/*
 * A175842  Partial sums of ceiling(n^2/14).
 * https://oeis.org/A175842
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{k=0}^n ceil(k^2/14) Recurrence: a(0) = 0, a(n) =
 *            a(n-1) + (n*n + 13)/14 (n ≥ 1)
 * Range:     u8 n <= 21, u16 n <= 139, u32 n <= 5649, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed values to avoid recomputation */
    static seq_t *cache = NULL;   /* cache[0] == a(0) */
    static size_t cap = 0;        /* allocated size */
    static size_t used = 0;       /* number of elements currently stored (≥1 if cache exists) */

    /* ensure array is large enough */
    if (cap < (size_t)n + 1) {
        size_t newcap = cap ? cap * 2 : 16;
        if (newcap < (size_t)n + 1)
            newcap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) {                /* allocation failed – abort */
            abort();
        }
        cache = tmp;
        cap = newcap;
    }

    /* compute missing entries up to n */
    if (used == 0) {               /* first call */
        cache[0] = 0;
        used = 1;
    }

    for (idx_t k = used; k <= n; ++k) {
        if (k == 0) {
            cache[0] = 0;
        } else {
            seq_t term = ((seq_t)k * (seq_t)k + (seq_t)13) / (seq_t)14;
            cache[(size_t)k] = cache[(size_t)(k - 1)] + term;
        }
    }
    used = (size_t)n + 1;
    return cache[(size_t)n];
}
