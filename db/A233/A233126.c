/*
 * A233126  Number of nX5 0..5 arrays with no element x(i,j) adjacent to
 *          itself or value 5-x(i,j) horizontally or vertically, top left
 *          element zero, and 1 appearing before 2 3 and 4, and 2 appearing
 *          before 3 in row major order (unlabelled 6-colorings with no
 *          clashing color pairs).
 * https://oeis.org/A233126
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 544*a(n-1) - 82944*a(n-2) + 5144576*a(n-3) -
 *            146800640*a(n-4) + 1879048192*a(n-5) - 8589934592*a(n-6) Initial
 *            values (n=1..6): a(1)=36, a(2)=10496, a(3)=3497984,
 *            a(4)=1189609472, a(5)=406142844928, a(6)=138791062863872 The
 *            function memoises computed terms and works for any call order
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

#ifdef __cplusplus
    /* C++ implementation using std::vector for memoisation */
    static std::vector<seq_t> cache = {
        (seq_t)36,                /* a(1) */
        (seq_t)10496,             /* a(2) */
        (seq_t)3497984,           /* a(3) */
        (seq_t)1189609472,        /* a(4) */
        (seq_t)406142844928,      /* a(5) */
        (seq_t)138791062863872    /* a(6) */
    };

    if (cache.size() < n) {
        for (idx_t idx = cache.size() + 1; idx <= n; ++idx) { /* idx is 1‑based */
            seq_t term =
                (seq_t)544        * cache[idx-2]      /* a(n-1) */
              - (seq_t)82944      * cache[idx-3]      /* a(n-2) */
              + (seq_t)5144576    * cache[idx-4]      /* a(n-3) */
              - (seq_t)146800640  * cache[idx-5]      /* a(n-4) */
              + (seq_t)1879048192 * cache[idx-6]      /* a(n-5) */
              - (seq_t)8589934592 * cache[idx-7];     /* a(n-6) */
            cache.push_back(term);
        }
    }
    return cache[n-1];

#else
    /* C implementation using a static heap array */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of stored terms */

    if (cache == NULL) {           /* initialise with first six values */
        cache_size = 6;
        cache = malloc((size_t)cache_size * sizeof(seq_t));
        if (!cache) return (seq_t)0;           /* allocation failure */
        cache[0] = (seq_t)36;
        cache[1] = (seq_t)10496;
        cache[2] = (seq_t)3497984;
        cache[3] = (seq_t)1189609472;
        cache[4] = (seq_t)406142844928;
        cache[5] = (seq_t)138791062863872;
    }

    if (n > cache_size) {
        seq_t *new_cache = realloc(cache, (size_t)n * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;        /* allocation failure */
        cache = new_cache;
        for (idx_t i = cache_size + 1; i <= n; ++i) { /* i is 1‑based */
            seq_t term =
                (seq_t)544        * cache[i-2]
              - (seq_t)82944      * cache[i-3]
              + (seq_t)5144576    * cache[i-4]
              - (seq_t)146800640  * cache[i-5]
              + (seq_t)1879048192 * cache[i-6]
              - (seq_t)8589934592 * cache[i-7];
            cache[i-1] = term;
        }
        cache_size = n;
    }
    return cache[n-1];
#endif
}
