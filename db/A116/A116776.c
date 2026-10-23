/*
 * A116776  Number of permutations of length n which avoid the patterns 2134,
 *          3142, 3421.
 * https://oeis.org/A116776
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 24, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Recurrence:  a(1)=1, a(2)=2, a(3)=6, a(4)=21, a(5)=73,
 *              a(6)=239, a(7)=734.
 *             For n ≥ 8:
 *        a(n) = 10*a(n-1) - 41*a(n-2) + 88*a(n-3) - 104*a(n-4)
 *               + 64*a(n-5) - 16*a(n-6).
 */
#include <stddef.h>
/* The utils.h file provides ensure_cache */
#include "../utils/ensure_cache.h"

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t computed_max = 0;

static const seq_t init[8] = {0,1,2,6,21,73,239,734};

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* ensure the array can hold indices up to n */
    if ((size_t)n + 1 > cache_cap)
        ensure_cache((idx_t)n + 1, &cache, &cache_cap);

    if (n <= computed_max)
        return cache[n];

    for (idx_t i = computed_max + 1; i <= n; ++i) {
        if (i <= 7) {
            cache[i] = init[i];
        } else {
            seq_t pos = 0, neg = 0;
            pos += 10 * cache[i-1];
            pos += 88 * cache[i-3];
            pos += 64 * cache[i-5];
            neg += 41 * cache[i-2];
            neg += 104 * cache[i-4];
            neg += 16 * cache[i-6];
            cache[i] = pos - neg;
        }
    }
    computed_max = n;
    return cache[n];
}
