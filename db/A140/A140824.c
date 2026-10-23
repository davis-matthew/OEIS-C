/*
 * A140824  Expansion of (x-x^3)/(1-3*x+2*x^2-3*x^3+x^4).
 * https://oeis.org/A140824
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 2*a(n-2) + 3*a(n-3) - a(n-4); a(0)=0, a(1)=1,
 *            a(2)=3, a(3)=6 (in-place recurrence, memoised)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 47, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* dynamic array of computed terms   */
static idx_t  len   = 0;      /* number of terms currently stored  */
static idx_t  cap   = 0;      /* current allocated capacity        */

/* Ensure capacity for "needed" terms.  The capacity grows exponentially
   to keep reallocs infrequent.  'new_cap' is at least needed. */
static void ensure_capacity(idx_t needed) {
    if (needed <= cap)
        return;
    idx_t new_cap = (cap > 0) ? cap : 4;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();   /* allocation failure */
    cache = tmp;
    cap   = new_cap;
}

/* Initialise the first four terms if they are not yet set.  The array
   is guaranteed to have cap >= 4 when this is called. */
static void init_base_terms(void) {
    cache[0] = (seq_t)0;
    cache[1] = (seq_t)1;
    cache[2] = (seq_t)3;
    cache[3] = (seq_t)6;
}

/* Populate the cache up to index n (inclusive).  New terms are generated
   with the recurrence.  This function may be called with n in any order. */
static void ensure_cache(idx_t n) {
    idx_t needed = n + 1;          /* number of entries needed          */
    ensure_capacity(needed);       /* make sure we can store them       */

    /* Initialise the base terms if we haven't yet computed them. */
    if (len < 4) {
        init_base_terms();
        /* The number of terms now stored is the smaller of 4 and needed. */
        len = (len < 4 && needed < 4) ? needed : 4;
    }

    /* Compute any missing terms up to n. */
    for (idx_t i = len; i <= n; ++i) {
        cache[i] = 3 * cache[i - 1]
                 - 2 * cache[i - 2]
                 + 3 * cache[i - 3]
                 -   cache[i - 4];
    }
    if (len < n + 1)
        len = n + 1;
}

seq_t a(idx_t n) {
    ensure_cache(n);
    return cache[n];
}
