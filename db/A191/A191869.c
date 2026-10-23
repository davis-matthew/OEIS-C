/*
 * A191869  First differences of the dying rabbits sequence A000044.
 * https://oeis.org/A191869
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=0, a(2)=0, a(3)=1, a(4)=1, a(5)=2, a(6)=3, a(7)=5, a(8)=8,
 *            a(9)=13, a(10)=21, a(11)=34; for n>11,
 *            a(n)=a(n-1)+a(n-3)+a(n-5)+a(n-7)+a(n-9)+a(n-11)
 * Range:     u8 n <= 15, u16 n <= 26, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Ensure compatibility with C11 and C++17.
    The function can be called in any order and will memoise
    computed values in a static array.  */
#include <stdlib.h>

static const seq_t init_vals[12] = {
    0,                      /* 0th element unused */
    0, 0, 1, 1, 2, 3, 5, 8, 13, 21, 34
};

static seq_t *cache = NULL;   /* memoised sequence */
static idx_t cache_len = 0;   /* highest index currently computed */
static idx_t cache_cap = 0;   /* allocated capacity (largest index usable) */

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* safety: offset starts at 1 */

    /* If already computed, return immediately. */
    if (cache && n <= cache_len) return cache[n];

    /* First call – allocate and initialise base terms. */
    if (!cache) {
        cache_cap = (n > 20 ? n + (n >> 1) : 20);
        cache = (seq_t*) calloc(cache_cap + 1, sizeof(seq_t));
        for (idx_t i = 1; i < 12; ++i) cache[i] = init_vals[i];
        cache_len = 11;
    }

    /* Grow cache if needed. */
    if (n > cache_cap) {
        idx_t new_cap = cache_cap;
        while (new_cap < n) new_cap <<= 1; /* grow by factor 2 */
        seq_t *tmp = (seq_t*) realloc(cache,
                                     (new_cap + 1) * sizeof(seq_t));
        if (!tmp) return 0;                /* allocation failure */
        for (idx_t i = cache_cap + 1; i <= new_cap; ++i) tmp[i] = 0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute new values up to n. */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        if (i <= 11) continue; /* base values already set            */
        cache[i] = cache[i-1] + cache[i-3] + cache[i-5]
                 + cache[i-7] + cache[i-9] + cache[i-11];
    }
    cache_len = n;

    return cache[n];
}
