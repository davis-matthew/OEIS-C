/*
 * A183426  Half the number of n X 3 0..2 arrays with no element equal its row
 *          sum plus its column sum mod 3.
 * https://oeis.org/A183426
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-1) + 7*a(n-3) - 56*a(n-4) + 8*a(n-6) - 64*a(n-7),
 *            with initial values a(1)=9, a(2)=32, a(3)=256, a(4)=2097,
 *            a(5)=16384, a(6)=131072, a(7)=1048959
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_last = 0;

static void ensure_cache(idx_t n) {
    if (cache_cap <= n) {
        size_t new_cap = n + 1;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) abort();
        for (size_t i = 0; i < cache_cap; ++i) new_cache[i] = cache[i];
        free(cache);
        cache = new_cache;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0; /* not used, but safe */
    ensure_cache(n);
    if (cache_last < n) {
        for (idx_t i = cache_last + 1; i <= n; ++i) {
            switch (i) {
                case 1: cache[1] = (seq_t)9; break;
                case 2: cache[2] = (seq_t)32; break;
                case 3: cache[3] = (seq_t)256; break;
                case 4: cache[4] = (seq_t)2097; break;
                case 5: cache[5] = (seq_t)16384; break;
                case 6: cache[6] = (seq_t)131072; break;
                case 7: cache[7] = (seq_t)1048959; break;
                default: {
                    seq_t sum1 = 8*cache[i-1] + 7*cache[i-3] + 8*cache[i-6];
                    seq_t sum2 = 56*cache[i-4] + 64*cache[i-7];
                    cache[i] = sum1 - sum2;
                }
            }
        }
        cache_last = n;
    }
    return cache[n];
}
