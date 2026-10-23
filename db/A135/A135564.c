/*
 * A135564  a(n) defined by a(2*n) = a(2*n-2) - (a(n) - 2*a(n-1) + a(n-2)) for
 *          n > 2, a(2*n+1) = a(2*n) - (a(n-2) - 2*a(n-3) + a(n-4)), for n >
 *          3, with a(0)=0, a(1)=1, a(2)=3, a(3)=-1, a(4)=-2, a(5)=-3, a(6)=4,
 *          a(7)=2.
 * https://oeis.org/A135564
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, less
 * Formula:   not stated
 * Range:     i8 n <= 252, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A135564
    Recurrence:
    a(0)=0, a(1)=1, a(2)=3, a(3)=-1,
    a(4)=-2, a(5)=-3, a(6)=4, a(7)=2
    a(2*n)   = a(2*n-2) - (a(n) - 2*a(n-1) + a(n-2))   for n>2
    a(2*n+1) = a(2*n)   - (a(n-2) - 2*a(n-3) + a(n-4))   for n>3
*/

#ifdef __cplusplus
#include <vector>
#endif
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
#ifdef __cplusplus
    /*  C++ implementation – use std::vector for automatic construction */
    static std::vector<seq_t> cache;
    /* lazily initialise the first eight terms */
    if (cache.empty()) {
        cache.reserve(8);
        cache.push_back((seq_t)0);   /* a(0) */
        cache.push_back((seq_t)1);   /* a(1) */
        cache.push_back((seq_t)3);   /* a(2) */
        cache.push_back((seq_t)-1);  /* a(3) */
        cache.push_back((seq_t)-2);  /* a(4) */
        cache.push_back((seq_t)-3);  /* a(5) */
        cache.push_back((seq_t)4);   /* a(6) */
        cache.push_back((seq_t)2);   /* a(7) */
    }

    if ((idx_t)cache.size() <= n) {
        /*  extend cache so it can hold a(n)  */
        for (size_t i = cache.size(); i <= (size_t)n; ++i) {
            idx_t i64 = (idx_t)i;
            if ((i64 & 1) == 0) {            /* even: a(2*n) */
                idx_t k = i64 >> 1;
                seq_t val = cache[i - 2]
                    - ( cache[k]
                      - (seq_t)2 * cache[k - 1]
                      + cache[k - 2] );
                cache.push_back(val);
            } else {                          /* odd: a(2*n+1) */
                idx_t k = (i64 - 1) >> 1;
                seq_t val = cache[i - 1]
                    - ( cache[k - 2]
                      - (seq_t)2 * cache[k - 3]
                      + cache[k - 4] );
                cache.push_back(val);
            }
        }
    }
    return cache[(size_t)n];
#else
    /*  C implementation – use malloc / realloc for arrays of integers */
    static seq_t *cache = NULL;
    static idx_t  cap   = 0;       /* capacity of cache array */
    static idx_t  last  = (idx_t)-1;/* largest index computed */

    /*  grow the cache if necessary */
    if (n >= cap) {
        idx_t new_cap = (cap > 0) ? cap : 8;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;    /* OOM – return 0 as a failure value */
        cache = tmp;
        cap   = new_cap;
    }

    /*  compute missing values up to n  */
    for (idx_t i = last + 1; i <= n; ++i) {
        if (i <= 7) {          /* explicit initial terms */
            switch (i) {
                case 0: cache[0] = (seq_t)0; break;
                case 1: cache[1] = (seq_t)1; break;
                case 2: cache[2] = (seq_t)3; break;
                case 3: cache[3] = (seq_t)-1; break;
                case 4: cache[4] = (seq_t)-2; break;
                case 5: cache[5] = (seq_t)-3; break;
                case 6: cache[6] = (seq_t)4; break;
                case 7: cache[7] = (seq_t)2; break;
            }
        } else if ((i & 1) == 0) {  /* even index */
            idx_t k = i >> 1;
            cache[i] = cache[i-2]
                - ( cache[k]
                  - (seq_t)2 * cache[k-1]
                  + cache[k-2] );
        } else {                    /* odd index */
            idx_t k = (i - 1) >> 1;
            cache[i] = cache[i-1]
                - ( cache[k-2]
                  - (seq_t)2 * cache[k-3]
                  + cache[k-4] );
        }
    }
    last = n;
    return cache[n];
#endif
}
