/*
 * A017886  Expansion of
 *          1/(1-x^9-x^10-x^11-x^12-x^13-x^14-x^15-x^16-x^17-x^18-x^19).
 * https://oeis.org/A017886
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{k=9}^{19} a(n-k) (a(0)=1, a(1..8)=0)
 * Range:     u8 n <= 44, u16 n <= 74, u32 n <= 135, u64 n <= 256,
 *            u128 n <= 498, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) counts compositions of n into parts 9..19. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache   = NULL;
    static size_t  cap    = 0;    /* allocated size of cache   */
    static idx_t   last   = 0;    /* largest index that is already computed */

    /* ---------- initialise cache on first call ---------- */
    if (cache == NULL) {
        /* ensure capacity for at least the first 20 indices */
        size_t init_cap = n + 1;
        if (init_cap < 20) init_cap = 20;
        cache = (seq_t *)calloc(init_cap, sizeof(seq_t));
        if (!cache) return 0;            /* allocation failed */
        /* a(0)=1 is already set by calloc (zero), set explicitly */
        cache[0] = 1;
        /* a(1..8)=0 are already zero from calloc */
        cap  = init_cap;
        last = 0;
    }

    /* ---------- grow cache if needed ---------- */
    if ((size_t)n >= cap) {
        size_t old_cap = cap;
        size_t new_cap = (cap ? cap * 2 : 20);
        if (new_cap <= (size_t)n) new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                /* allocation failed */
        /* zero‑initialise newly added slots */
        for (size_t i = old_cap; i < new_cap; ++i) tmp[i] = 0;
        cache = tmp;
        cap   = new_cap;
    }

    /* ---------- return cached value if already computed ---------- */
    if (n <= last) return cache[n];

    /* ---------- compute missing values up to n ---------- */
    idx_t start = last + 1;
    if (start < 1) start = 1;            /* a(0) is already set */

    for (idx_t i = start; i <= n; ++i) {
        seq_t sum = 0;
        /* sum over k = 9 .. 19 */
        for (int k = 9; k <= 19; ++k) {
            if (i >= (idx_t)k) {
                sum += cache[i - (idx_t)k];
            }
        }
        cache[i] = sum;
    }
    last = n;

    return cache[n];
}
