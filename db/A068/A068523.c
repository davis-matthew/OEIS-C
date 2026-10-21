/*
 * A068523  Essentially the same as A055500.
 * https://oeis.org/A068523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = largest prime ≤ a(n-1) + a(n-2), with a(1)=2, a(2)=3
 * Range:     i8 n <= 10, i16 n <= 20, i32 n <= 20, i64 n <= 20, i128 n <= 20,
 *            big any n
 * Helpers:   is_prime_big, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/seq_realloc.h"

static seq_t *cache = NULL;
static idx_t cache_len = 0;   /* number of terms currently stored */
static size_t cache_cap = 0;  /* allocated capacity */

/* ensure the cache can hold at least `needed` elements (where needed ≤ 2^64) */
static void ensure_capacity(idx_t needed)
{
    if ((idx_t)cache_cap >= needed) return;
    size_t new_cap = needed > 64 ? needed : 64;
    while ((idx_t)new_cap < needed) new_cap <<= 1;
    seq_t *new_cache = seq_realloc(cache, cache_cap, new_cap);
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < (idx_t)1) return (seq_t)0;     /* undefined for n<1 */

    if (cache_len < n) {
        ensure_capacity(n);
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            if (i == (idx_t)1) {
                cache[0] = (seq_t)2;
            } else if (i == (idx_t)2) {
                cache[1] = (seq_t)3;
            } else {
                seq_t sum      = cache[i - 2] + cache[i - 3];
                seq_t candidate = sum;
                while (!is_prime_big(candidate)) {
                    candidate -= (seq_t)1;
                }
                cache[i - 1] = candidate;
            }
        }
        cache_len = n;
    }
    return cache[n - 1];
}
