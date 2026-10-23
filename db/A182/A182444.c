/*
 * A182444  a(0)=0, a(1)=1, for n>1, a(n) = a(n-1) + (a(n-2) mod n).
 * https://oeis.org/A182444
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0, a(1) = 1, for n > 1, a(n) = a(n-1) + (a(n-2) mod n)
 * Range:     u8 n <= 29, u16 n <= 511, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache ------------------------------------------------------- */
    static seq_t *cache = NULL;      /* array holding computed terms       */
    static size_t  cap   = 0;        /* allocated capacity (number of cells) */
    static size_t  len   = 0;        /* highest index + 1 that has been computed */

    /* Convert the index to a type safe for array indexing. */
    size_t sn = (size_t)n;

    /* Initialise cache on the first call. */
    if (cap == 0) {
        cap = 2;                          /* start with capacity for indices 0,1 */
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;       /*  out‑of‑memory fallback             */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        len = 2;                          /* indices 0 and 1 are now available    */
    }

    /* If the requested index is already computed, return immediately. */
    if (sn < len) {
        return cache[sn];
    }

    /* Grow the allocation if necessary. */
    if (sn >= cap) {
        size_t new_cap = cap;
        while (sn >= new_cap) {
            new_cap *= 2;
        }
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;       /*  out‑of‑memory fallback             */
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute missing terms up to sn. */
    for (size_t i = len; i <= sn; ++i) {
        seq_t mod_val = cache[i - 2] % (seq_t)i;   /* a(i-2) mod i              */
        cache[i] = cache[i - 1] + mod_val;         /* a(i) = a(i-1)+mod_val      */
    }
    len = sn + 1;    /* update the range that has been computed */

    /* Return the requested term. */
    return cache[sn];
}
