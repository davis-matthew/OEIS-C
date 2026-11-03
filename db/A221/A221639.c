/*
 * A221639  a(n) = 5^(5*n+1) + 4^(5*n+2) + 3^(5*n).
 * https://oeis.org/A221639
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^(5*n+1) + 4^(5*n+2) + 3^(5*n)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 5, u128 n <= 10,
 *            big any n
 * Validated: standard, pass, 28 terms through n = 256 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence (derived from generating function):
   a(0) = 22,       a(1) = 32252,      a(2) = 65664390
   a(n) = 4392*a(n-1) - 4208207*a(n-2) + 777600000*a(n-3)   (n >= 3) */

#include <stddef.h>
#include <stdlib.h>

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cap   = 0;     /* allocated capacity */
    static idx_t  last  = 0;     /* highest index already computed */

    /* Base cases */
    if (n == 0) return (seq_t)22;
    if (n == 1) return (seq_t)32252;
    if (n == 2) return (seq_t)65664390;

    /* Initial allocation */
    if (cache == NULL) {
        cap = (n + 1 > 3 ? n + 1 : 3);   /* at least 3 for the bases */
        cache = seq_alloc(cap);
        cache[0] = (seq_t)22;
        cache[1] = (seq_t)32252;
        cache[2] = (seq_t)65664390;
        last = 2;
    }
    /* Grow the cache if needed */
    else if ((size_t)(n + 1) > cap) {
        cache = seq_realloc(cache, cap, n + 1);
        cap   = n + 1;
    }

    /* Return cached value if already computed */
    if (n <= last) return cache[n];

    /* Compute missing terms using the recurrence */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t t1 = cache[i - 1] * (seq_t)4392;
        seq_t t2 = cache[i - 2] * (seq_t)4208207;
        seq_t t3 = cache[i - 3] * (seq_t)777600000;
        cache[i] = t1 - t2 + t3;
    }
    last = n;
    return cache[n];
}
