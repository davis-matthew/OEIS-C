/*
 * A045696  Number of ternary words of length n (beginning 0) with
 *          autocorrelation function 2^(n-1)+2.
 * https://oeis.org/A045696
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=0, a(2)=0, a(3)=0, a(4)=2; a(2*n-1)=3*a(2*n-2)+3*a(n)
 *            (n>=3); a(2*n)= 3*a(2*n-1)-a(n)-a(n+1) (n>=3)
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 23, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
#include <vector>
#else
#include <stdlib.h>
#endif

#if defined(__cplusplus)
static std::vector<seq_t> cache;
#else
static seq_t *cache = NULL;
static idx_t cache_len = 0;
#endif

seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* undefined for n<1, but not required by spec */

#if defined(__cplusplus)
    /* Ensure enough storage */
    if (cache.size() < (size_t)(n + 1))
        cache.resize((size_t)(n + 1));

#else
    if (cache_len <= n) {
        size_t new_len = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) return 0;            /* allocation failure: unlikely */
        cache = new_cache;
        cache_len = new_len;
    }
#endif

    /* Compute values up to n */
    seq_t three = (seq_t)3;
#if defined(__cplusplus)
    for (size_t i = 1; i <= (size_t)n; ++i) {
        if (i <= 4) {
            switch ((int)i) {
                case 1: cache[i] = 0; break;
                case 2: cache[i] = 0; break;
                case 3: cache[i] = 0; break;
                case 4: cache[i] = 2; break;
            }
        } else if ((i & 1) == 1) {                /* odd */
            size_t k = (i + 1) / 2;
            cache[i] = three * cache[i - 1] + three * cache[k];
        } else {                                 /* even */
            size_t k = i / 2;
            cache[i] = three * cache[i - 1] - cache[k] - cache[k + 1];
        }
    }
    return cache[(size_t)n];

#else
    for (idx_t i = 1; i <= n; ++i) {
        if (i <= 4) {
            switch ((int)i) {
                case 1: cache[i] = 0; break;
                case 2: cache[i] = 0; break;
                case 3: cache[i] = 0; break;
                case 4: cache[i] = 2; break;
            }
        } else if ((i & 1) == 1) {                /* odd */
            idx_t k = (i + 1) / 2;
            cache[i] = three * cache[i - 1] + three * cache[k];
        } else {                                 /* even */
            idx_t k = i / 2;
            cache[i] = three * cache[i - 1] - cache[k] - cache[k + 1];
        }
    }
    return cache[n];
#endif
}
