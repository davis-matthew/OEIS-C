/*
 * A232909  Number of (1+1) X (n+1) 0..2 arrays with every element next to
 *          itself plus and minus one within the range 0..2 horizontally or
 *          antidiagonally.
 * https://oeis.org/A232909
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=0, a(2)=10, a(3)=52, a(4)=82, a(5)=432, a(6)=2372,
 *            a(7)=8456, a(8)=29970 a(n) = 4*a(n-2) + 28*a(n-3) + 80*a(n-4) +
 *            48*a(n-5) - 67*a(n-6) - 228*a(n-7) - 224*a(n-8) for all n ≥ 9
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_cap = 0;   /* number of elements allocated (>= n+1) */
static idx_t last_prepared = 0; /* highest n for which a(n) is known */

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* n < 1 is undefined, return 0 */

    /* Prepare cache up to n */
    if (cache == NULL) {
        /* Allocate at least space for indices 0..8 (9 elements) */
        cache_cap = 9;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;   /* allocation failed */

        /* initialise base values (indices 1..8) */
        cache[0] = 0;  /* unused */
        cache[1] = 0;
        cache[2] = 10;
        cache[3] = 52;
        cache[4] = 82;
        cache[5] = 432;
        cache[6] = 2372;
        cache[7] = 8456;
        cache[8] = 29970;
        last_prepared = 8;
    }

    if (n >= cache_cap) {
        /* grow cache to accommodate index n */
        idx_t new_cap = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) return 0;   /* reallocation failed */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* compute missing values up to n */
    for (idx_t i = last_prepared + 1; i <= n; ++i) {
        if (i <= 8) {
            /* base values are already set */
            continue;
        }
        cache[i] =
            4   * cache[i-2] + 28 * cache[i-3] + 80 * cache[i-4] + 48 * cache[i-5]
          - 67 * cache[i-6] - 228 * cache[i-7] - 224 * cache[i-8];
    }
    last_prepared = (n > last_prepared) ? n : last_prepared;

    return cache[n];
}
