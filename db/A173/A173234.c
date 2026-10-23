/*
 * A173234  Expansion of
 *          x*(1+3*x^2-2*x^3+2*x^4-x^5)/((1+x)*(x-1)^2*(x^2+1)^2).
 * https://oeis.org/A173234
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, less, changed
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A173234
   a(n) = a(n-1) - a(n-2) + a(n-3) + a(n-4) - a(n-5) + a(n-6) - a(n-7)
   with initial values:
   a(0)=0, a(1)=1, a(2)=1, a(3)=3, a(4)=1, a(5)=2, a(6)=3
   The function caches computed values so that any call order is supported.
*/

#if defined(__cplusplus)
#  include <vector>
#else
#  include <stdlib.h>
#endif

seq_t a(idx_t n)
{
#if defined(__cplusplus)
    static std::vector<seq_t> cache;
    static idx_t cached_len = 0;
#else
    static seq_t *cache = NULL;
    static idx_t cached_len = 0;
#endif

    /* initialise base values on first run */
    if (cached_len == 0) {
#if defined(__cplusplus)
        cache.resize(7);
        cache[0] = 0;
        cache[1] = 1;
        cache[2] = 1;
        cache[3] = 3;
        cache[4] = 1;
        cache[5] = 2;
        cache[6] = 3;
#else
        cache = (seq_t *)malloc(7 * sizeof(seq_t));
        if (!cache) return 0;          /* allocation failed */
        cache[0] = 0;
        cache[1] = 1;
        cache[2] = 1;
        cache[3] = 3;
        cache[4] = 1;
        cache[5] = 2;
        cache[6] = 3;
#endif
        cached_len = 7;
    }

    /* extend cache if necessary */
    if (n >= cached_len) {
#if defined(__cplusplus)
        cache.resize(n + 1);
#else
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp) {
            /* allocation failed */
            return 0;
        }
        cache = tmp;
#endif
        for (idx_t i = cached_len; i <= n; ++i) {
            /* compute a(i) using unsigned arithmetic (wrap‑around is safe here) */
            seq_t v1 = cache[i - 1];
            seq_t v2 = cache[i - 2];
            seq_t v3 = cache[i - 3];
            seq_t v4 = cache[i - 4];
            seq_t v5 = cache[i - 5];
            seq_t v6 = cache[i - 6];
            seq_t v7 = cache[i - 7];
#if defined(__cplusplus)
            /* use addition/subtraction directly on seq_t (supports big int types) */
            seq_t val = v1 - v2 + v3 + v4 - v5 + v6 - v7;
#else
            /* unsigned arithmetic */
            seq_t val = v1;
            val -= v2;
            val += v3;
            val += v4;
            val -= v5;
            val += v6;
            val -= v7;
#endif
            cache[i] = val;
        }
        cached_len = n + 1;
    }

    return cache[n];
}
