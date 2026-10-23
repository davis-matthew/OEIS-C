/*
 * A011729  A binary m-sequence: expansion of reciprocal of
 *          x^16+x^5+x^3+x^2+1.
 * https://oeis.org/A011729
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-16) ^ a(n-5) ^ a(n-3) ^ a(n-2), with initial terms
 *            a(0)=0,...,a(14)=0, a(15)=1 (period 65535)
 * Range:     u8 n <= 9916, u16 n <= 9916, u32 n <= 9916, u64 n <= 9916,
 *            u128 n <= 9916, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoization */
    static seq_t *cache = NULL;      /* array of computed terms   */
    static size_t cap   = 0;         /* current allocation size   */
    static size_t len   = 0;         /* number of terms stored    */

    /* Lazy initialization of cache and first 16 terms */
    if (cap == 0) {
        cap = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* allocation failure: return 0 */
        for (size_t i = 0; i < 16; ++i) {
            cache[i] = (i == 15) ? (seq_t)1 : (seq_t)0;
        }
        len = 16;
    }

    size_t idx = (size_t)n;

    /* Grow the cache if needed */
    if (idx >= cap) {
        size_t newcap = cap;
        while (newcap <= idx) newcap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* allocation failure: return 0 */
        cache = tmp;
        cap = newcap;
    }

    /* Compute missing terms up to idx using the recurrence */
    if (idx >= len) {
        for (size_t i = len; i <= idx; ++i) {
            cache[i] =
                cache[i - 16] ^ cache[i - 5] ^ cache[i - 3] ^ cache[i - 2];
        }
        len = idx + 1;
    }

    return cache[idx];
}
