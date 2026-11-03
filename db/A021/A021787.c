/*
 * A021787  Decimal expansion of 1/783.
 * https://oeis.org/A021787
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10^(n+1)/783) mod 10
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Validated: partial, pass, 99 terms through n = 98 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Make sure the cache is large enough */
static void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr) {
    seq_t *cache = *cache_ptr;
    size_t cap = *cap_ptr;
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? (cap << 1) : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp && new_cap > 0) {
            /* Allocation failed – abort the program. */
            exit(1);
        }
        if (tmp) {
            /* Initialize newly allocated slots to zero. */
            if (!cache) {
                for (size_t i = 0; i < new_cap; ++i)
                    tmp[i] = 0;
            } else {
                for (size_t i = cap; i < new_cap; ++i)
                    tmp[i] = 0;
            }
            *cache_ptr = tmp;
            *cap_ptr = new_cap;
        }
    }
}

static seq_t *digit_cache = NULL;
static idx_t cache_len = 0;
static size_t cache_cap = 0;
static unsigned long long rem = 1ULL;

seq_t a(idx_t n)
{
    if (cache_len <= n) {
        idx_t need = n + 1;
        ensure_cache(need, &digit_cache, &cache_cap);
        for (idx_t i = cache_len; i <= n; ++i) {
            unsigned long long temp = rem * 10ULL;
            seq_t digit = (seq_t)(temp / 783ULL);
            rem = temp % 783ULL;
            digit_cache[i] = digit;
        }
        cache_len = need;
    }
    return digit_cache[n];
}
