/*
 * A204771  a(n) = a(n-1) XOR (a(n-2)*3).
 * https://oeis.org/A204771
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = a(n-1) XOR (a(n-2)*3), a(0) = 0, a(1) = 1
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 35, u64 n <= 67, u128 n <= 131,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;   // number of terms already computed
static idx_t cache_cap = 0;   // allocated capacity

static void ensure_cache(idx_t n)
{
    if (n + 1 <= cache_cap)
        return;
    idx_t new_cap = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
    if (!tmp) abort();  /* allocation failed */
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    if (n == 1)
        return 1;

    ensure_cache(n);

    if (cache_len == 0) {
        cache[0] = 0;
        cache_len = 1;
    }
    if (cache_len == 1) {
        cache[1] = 1;
        cache_len = 2;
    }

    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t prev1 = cache[i - 1];
        seq_t prev2 = cache[i - 2];
        cache[i] = prev1 ^ (prev2 * 3);
    }
    cache_len = n + 1;
    return cache[n];
}
