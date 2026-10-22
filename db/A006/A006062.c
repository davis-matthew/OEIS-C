/*
 * A006062  Star-hex numbers.
 * https://oeis.org/A006062
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 34*a(n-1) - a(n-2) + 4, a(1) = 1, a(2) = 37
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t computed_up_to = 0;

seq_t a(idx_t n) {
    if (n < 1) return 0;

    if (cache_len <= n || cache_len < 3) {
        idx_t new_len = (cache_len == 0) ? 256 : cache_len;
        while (new_len <= n) new_len *= 2;

        seq_t *new_cache = (seq_t *)malloc(new_len * sizeof(seq_t));
        for (idx_t i = 0; i < new_len; i++) new_cache[i] = 0;
        if (cache != NULL) {
            for (idx_t i = 0; i < cache_len; i++) new_cache[i] = cache[i];
            free(cache);
        }
        cache = new_cache;
        cache_len = new_len;
    }

    if (computed_up_to == 0) {
        cache[1] = 1;
        cache[2] = 37;
        computed_up_to = 2;
    }

    if (computed_up_to < n) {
        for (idx_t i = computed_up_to + 1; i <= n; i++) {
            cache[i] = cache[i - 1] * 34 - cache[i - 2] + 4;
        }
        computed_up_to = n;
    }

    return cache[n];
}
