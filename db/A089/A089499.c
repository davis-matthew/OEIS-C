/*
 * A089499  a(0)=0; a(1)=1; a(2n) = 4*Sum_{k=0..n} a(2k-1); a(2n+1) = a(2n) +
 *          a(2n-1).
 * https://oeis.org/A089499
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 25, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A089499
   Recurrence:
       a(0) = 0
       a(1) = 1
       For n ≥ 1:
           a(2n)   = 4 * Σ_{k=0}^{n} a(2k-1)
           a(2n+1) = a(2n) + a(2n-1)
   The recurrence is implemented efficiently by keeping a running sum of all
   odd‑indexed terms seen so far.  This allows each term to be computed in
   O(1) time, giving an overall O(n) algorithm.  Values are memoised in a
   static array so that calls for arbitrary n return immediately after the
   needed terms have been generated once.
*/
#include <stdlib.h>

/* The types seq_t and idx_t are provided by the build system. */
seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;     /* array of values a(0)...a(k) */
    static size_t cap   = 0;        /* allocated capacity */
    static size_t len   = 0;        /* number of elements currently stored */
    static seq_t sum_odd = 0;       /* Σ of odd‑indexed terms seen so far */

    /* initialise with a(0)=0, a(1)=1 */
    if (!len) {
        /* allocate at least two slots */
        size_t init_cap = 8;
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        cap = init_cap;
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        len = 2;
        sum_odd = cache[1];          /* only a(1) contributes to the odd sum */
    }

    /* grow cache and compute missing terms up to n */
    if ((size_t)n >= len) {
        /* ensure capacity */
        size_t required = ((size_t)n + 1);
        if (required > cap) {
            size_t new_cap = cap ? cap * 2 : 8;
            while (new_cap < required) new_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache = tmp;
            cap = new_cap;
        }
        /* compute terms from len up to n */
        for (size_t i = len; i <= (size_t)n; ++i) {
            if ((i & 1) == 0) {          /* even index: a(2m) */
                cache[i] = (seq_t)4 * sum_odd;
            } else if (i == 1) {         /* already stored, safety */
                /* nothing to do, a[1] already set */
            } else {                      /* odd index: a(2m+1) */
                cache[i] = cache[i-1] + cache[i-2];
                sum_odd += cache[i];
            }
        }
        len = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
