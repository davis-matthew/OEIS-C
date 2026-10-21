/*
 * A222105  Number of binary arrays indicating the locations of trailing edge
 *          maxima of a random length-n 0..2 array extended with zeros and
 *          convolved with 1,2,2,1.
 * https://oeis.org/A222105
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 28, u32 n <= 58, u64 n <= 117,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence from OEIS A222105 :
   a(n) = a(n-1) + a(n-4) + 4*a(n-7) - 3*a(n-8)
          - 2*a(n-11) - 5*a(n-14) + 3*a(n-15)
          + a(n-18) + 2*a(n-21) - a(n-22)
   with initial values a(1)..a(22) as given. */
#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

static const seq_t init[22] = {
    2,   3,   4,   5,   8,   12,  17,  24,  36,  53,
    76,  109, 161, 236, 342, 498, 729, 1064,1545,2256,
    3296,4807
};

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined for offset 1 */

#ifdef __cplusplus
    /* C++ using std::vector to handle non‑trivially copyable types. */
    static std::vector<seq_t> cache;
    if (cache.size() < n) {
        size_t old_size = cache.size();
        cache.resize(n);               /* new elements default‑constructed */
        for (idx_t i = old_size; i < n; ++i) {
            if (i < 22) {
                cache[i] = init[i];
            } else {
                seq_t pos = cache[i-1] + cache[i-4] + cache[i-18] + cache[i-21]*2
                            + cache[i-7]*4 + cache[i-15]*3;
                seq_t sub = cache[i-8]*3 + cache[i-11]*2 + cache[i-14]*5
                            + cache[i-22];
                cache[i] = pos - sub;
            }
        }
    }
    return cache[n-1];
#else
    /* C using realloc (trivially copyable types). */
    static seq_t *cache = NULL;
    static idx_t cached_len = 0;
    if (cached_len < n) {
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)n * sizeof(seq_t));
        if (!tmp) abort();              /* allocation failure */
        cache = tmp;
        for (idx_t i = cached_len; i < n; ++i) {
            if (i < 22) {
                cache[i] = init[i];
            } else {
                seq_t pos = cache[i-1] + cache[i-4] + cache[i-18] + cache[i-21]*2
                            + cache[i-7]*4 + cache[i-15]*3;
                seq_t sub = cache[i-8]*3 + cache[i-11]*2 + cache[i-14]*5
                            + cache[i-22];
                cache[i] = pos - sub;
            }
        }
        cached_len = n;
    }
    return cache[n-1];
#endif
}
