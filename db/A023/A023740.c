/*
 * A023740  Numbers with exactly 3 4's in base 5 expansion.
 * https://oeis.org/A023740
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   count_fours_base5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: search through consecutive integers and keep those whose base‑5
   representation contains exactly three digits equal to 4.  The result is
   cached so that later calls do not need to re‑search already inspected
   values. */

#include <stdlib.h>   /* exit, realloc */
#include "../utils/count_fours_base5.h"

static seq_t *cache   = NULL;   /* stored terms   */
static size_t  cap    = 0;      /* allocated size */
static idx_t   count  = 0;      /* number of cached terms (a(1) is cache[0]) */
static seq_t  last_val = (seq_t)0;   /* last integer tested */

/* Grow the cache to at least new_cap elements.  In C++ we use operator new
   and delete; in C we fall back to realloc. */
static void grow_cache(size_t new_cap) {
#ifdef __cplusplus
    seq_t *newp = new seq_t[new_cap];
    if (cache) {
        for (size_t i = 0; i < count; ++i)
            newp[i] = cache[i];
        delete [] cache;
    }
    cache = newp;
    cap = new_cap;
#else
    void *p = realloc(cache, new_cap * sizeof(seq_t));
    if (!p) exit(1);
    cache = (seq_t *)p;
    cap = new_cap;
#endif
}

seq_t a(idx_t n) {
    /* n is 1‑based offset; behaviour for n < 1 is undefined. */
    if (n <= count)
        return cache[n - 1];

    /* Extend cache until at least n terms are available. */
    while (count < n) {
        if (count_fours_base5(last_val) == 3) {
            /* Ensure enough space for a new entry. */
            if (count >= cap)
                grow_cache((cap == 0) ? 4 : cap * 2);
            cache[count++] = last_val;
        }
        ++last_val;   /* Advance to the next integer. */
    }
    return cache[n - 1];
}
