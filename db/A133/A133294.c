/*
 * A133294  a(n) = 2*a(n-1) + 10*a(n-2), a(0)=1, a(1)=1.
 * https://oeis.org/A133294
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=1, a(n)=2*a(n-1)+10*a(n-2)   (n≥2) */
#include <stdlib.h>

/* On any invocation, returns the n‑th term of OEIS A133294.
   The function keeps a memoisation cache that grows lazily.  All
   computations are performed with the seq_t type supplied by the
   build, so the implementation is portable to 8‑, 16‑, 32‑, 64‑, 128‑bit
   integers as well as arbitrary precision builds. */
seq_t a(idx_t n)
{
    /* static cache holding computed values */
    static seq_t *cache = NULL;          /* array of seq_t values */
    static size_t cap   = 0;             /* allocated capacity */
    static size_t comp  = 0;             /* first index not yet computed */

    /* index within the cache and needed cache size */
    size_t idx  = (size_t)n;
    size_t need = idx + 1;

    /* grow cache if necessary */
    if (need > cap) {
        size_t newcap = cap ? cap << 1 : 1;
        while (newcap < need) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) exit(1);          /* allocation failed */
        cache = tmp;
        cap   = newcap;
    }

    /* ensure the cache can store the two base values before writing them */
    if (comp < 2 && cap < 2) {
        /* current cached capacity is less than 2 – grow it explicitly */
        size_t newcap = 2;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cap   = newcap;
    }

    /* initialise base values if not yet done */
    if (comp < 2) {
        cache[0] = 1;
        cache[1] = 1;
        comp     = 2;                 /* next index to compute */
    }

    /* if the requested value is already computed return it */
    if (idx < comp) return cache[idx];

    /* compute missing values up to idx */
    for (size_t i = comp; i <= idx; ++i) {
        cache[i] = 2 * cache[i - 1] + 10 * cache[i - 2];
    }
    comp = idx + 1;                   /* update next index to compute */

    return cache[idx];
}
