/*
 * A042275  Denominators of continued fraction convergents to sqrt(663).
 * https://oeis.org/A042275
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 35, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A042275 – Denominators of continued‑fraction convergents to sqrt(663)
    Formula: a(n) = 206*a(n-4) - a(n-8)  (for n > 7)
    Initial values:
        a(0)=1, a(1)=1, a(2)=3, a(3)=4,
        a(4)=203, a(5)=207, a(6)=617, a(7)=824
    The function is memoised and works for any order of calls.  */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache and bookkeeping */
    static seq_t *cache = NULL;
    static size_t capacity = 0;
    static idx_t max_computed = 0;

    /* initialise base table on first call */
    if (cache == NULL) {
        capacity = 8;                       /* at least accommodate the 8 seeds */
        cache = (seq_t*)malloc(capacity * sizeof(seq_t));
        if (!cache) return (seq_t)0;        /* allocation failed – return 0 */
        cache[0] = 1;  /* a(0) */
        cache[1] = 1;  /* a(1) */
        cache[2] = 3;  /* a(2) */
        cache[3] = 4;  /* a(3) */
        cache[4] = 203;/* a(4) */
        cache[5] = 207;/* a(5) */
        cache[6] = 617;/* a(6) */
        cache[7] = 824;/* a(7) */
        max_computed = 7;
    }

    /* grow the cache if the caller requests a larger index */
    if (n >= capacity) {
        size_t new_capacity = capacity * 2;
        if (new_capacity <= (size_t)n + 1)
            new_capacity = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_capacity * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failed – return 0 */
        cache = tmp;
        capacity = new_capacity;
    }

    /* if the value is already known, return it immediately */
    if (n <= max_computed)
        return cache[n];

    /* compute missing terms using the recurrence */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        if (i >= 8) {
            cache[i] = (seq_t)206 * cache[i - 4] - cache[i - 8];
        }   /* for i<8 the seed values are already stored */
    }
    max_computed = n;
    return cache[n];
}
