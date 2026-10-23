/*
 * A041205  Denominators of continued fraction convergents to sqrt(113).
 * https://oeis.org/A041205
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 1552*a(n-9) + a(n-18) Initial values: a(0)=1, a(1)=1,
 *            a(2)=2, a(3)=3, a(4)=8, a(5)=19, a(6)=27, a(7)=46, a(8)=73,
 *            a(9)=1506, a(10)=1579, a(11)=3085, a(12)=4664, a(13)=12413,
 *            a(14)=29490, a(15)=41903, a(16)=71393, a(17)=113296
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 28, u64 n <= 55, u128 n <= 109,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;        /* memoised values */
static idx_t cache_size = 0;       /* allocated size of cache */
static idx_t computed = 0;         /* number of entries already computed (cache[0..computed-1]) */

/* first 18 terms */
static const seq_t base_vals[18] = {
    1ULL, 1ULL, 2ULL, 3ULL, 8ULL, 19ULL, 27ULL, 46ULL, 73ULL,
    1506ULL, 1579ULL, 3085ULL, 4664ULL, 12413ULL, 29490ULL,
    41903ULL, 71393ULL, 113296ULL
};

seq_t a(idx_t n)
{
    /* ensure cache has at least n+1 entries */
    if (!cache) {
        cache_size = (n + 1 > 18 ? n + 1 : 18);
#if defined(__cplusplus)
        cache = reinterpret_cast<seq_t*>(malloc(sizeof(seq_t) * cache_size));
#else
        cache = (seq_t*)malloc(sizeof(seq_t) * cache_size);
#endif
        if (!cache) abort();
    } else if (n + 1 > cache_size) {
        idx_t new_size = n + 1;
#if defined(__cplusplus)
        seq_t *tmp = reinterpret_cast<seq_t*>(realloc(cache, sizeof(seq_t) * new_size));
#else
        seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * new_size);
#endif
        if (!tmp) abort();
        cache = tmp;
        cache_size = new_size;
    }

    /* fill missing entries up to n */
    for (idx_t i = computed; i <= n; ++i) {
        if (i < 18) {
            cache[i] = base_vals[i];
        } else {
            cache[i] = (seq_t)1552 * cache[i - 9] + cache[i - 18];
        }
    }
    computed = n + 1;
    return cache[n];
}
