/*
 * A021787  Decimal expansion of 1/783.
 * https://oeis.org/A021787
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10^(n+1)/783) mod 10
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/ensure_cache.h"

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
