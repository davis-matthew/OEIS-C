/*
 * A230736  Number of (n+3)X(2+3) 0..2 black square subarrays x(i,j) with each
 *          element diagonally or antidiagonally next to at least one element
 *          with value (x(i,j)+1) mod 3, and upper left element zero.
 * https://oeis.org/A230736
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(n) = 31*a(n-2) - 126*a(n-4) + 42*a(n-6) + 79*a(n-8)
          - a(n-10) + 8*a(n-12)
   Initial values (n = 1 … 12):
     8, 66, 244, 2016, 6576, 54138, 173428, 1427040,
     4558536, 37506738, 119758148, 985334832
*/
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cap = 0;   /* allocated capacity */
    static size_t max_n = 0; /* highest index already computed (1‑based) */

    /* base values */
    static const seq_t base[12] = {
        (seq_t)8ULL,
        (seq_t)66ULL,
        (seq_t)244ULL,
        (seq_t)2016ULL,
        (seq_t)6576ULL,
        (seq_t)54138ULL,
        (seq_t)173428ULL,
        (seq_t)1427040ULL,
        (seq_t)4558536ULL,
        (seq_t)37506738ULL,
        (seq_t)119758148ULL,
        (seq_t)985334832ULL
    };

    if (n == 0) return (seq_t)0;      /* undefined but safe */

    /* initialise cache if it does not exist */
    if (!cache) {
        /* always allocate at least 12 entries so the initial values
           are available for all future computations */
        size_t init = n > 12 ? n : 12;
        cache = (seq_t *)malloc(init * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* allocation failure */
        for (size_t i = 0; i < 12; ++i)
            cache[i] = base[i];
        cap = init;
        max_n = 12; /* we have the first 12 terms ready */
    }

    /* grow the cache if needed */
    if (n > cap) {
        size_t new_cap = cap * 2;
        if (new_cap < n) new_cap = n;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) return (seq_t)0; /* allocation failure */
        cache = new_cache;
        cap = new_cap;
    }

    /* compute missing terms up to n */
    if (n > max_n) {
        for (size_t k = max_n + 1; k <= n; ++k) {
            if (k <= 12) {
                /* base values already set */
                continue;
            }
            /* recurrence uses terms up to a(k-12) */
            seq_t val = (seq_t)31 * cache[k-3]
                      - (seq_t)126 * cache[k-5]
                      + (seq_t)42  * cache[k-7]
                      + (seq_t)79  * cache[k-9]
                      -            cache[k-11]
                      + (seq_t)8   * cache[k-13];
            cache[k-1] = val;
        }
        max_n = n;
    }

    return cache[n-1];
}
