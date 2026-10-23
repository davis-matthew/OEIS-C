/*
 * A146887  a(n) = prime(n)*a(n-1) - a(n-2).
 * https://oeis.org/A146887
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = prime(n)*a(n-1) - a(n-2), a(0)=1, a(1)=1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 26,
 *            big any n
 * Helpers:   ensure_cache, get_prime, seq_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/ensure_cache.h"
#include "../utils/get_prime.h"
#include "../utils/seq_one.h"

seq_t a(idx_t n)
{
    if (n == 0) return seq_one();
    if (n == 1) return seq_one();

    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t max_computed = 1;

    ensure_cache(n, &cache, &cache_cap);

    if (max_computed == 1) {
        cache[0] = seq_one();
        cache[1] = seq_one();
    }

    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t p   = (seq_t)get_prime(i);
        seq_t prod = p * cache[i - 1];
        cache[i]   = prod - cache[i - 2];
    }
    max_computed = n;
    return cache[n];
}
