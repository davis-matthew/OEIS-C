/*
 * A239293  Smallest composite c > n such that n^c == n (mod c).
 * https://oeis.org/A239293
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest composite c>n such that n^c == n (mod c)
 * Range:     u8 n <= 1, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   ensure_cache, is_prime_small, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_small.h"
#include "../utils/mod_pow.h"
#include "../utils/ensure_cache.h"

static seq_t *cache = NULL;
static size_t cache_cap = 0;

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* offset starts at 1 */
    ensure_cache(n, &cache, &cache_cap);     /* ensure cache size >= n+1 */

    if (cache[n] != 0) return cache[n];      /* memoised value */

    /* quick rule for even n : if n+1 is composite then a(n)=n+1 */
    if ((n & 1U) == 0 && !is_prime_small((seq_t)(n + 1))) {
        cache[n] = (seq_t)(n + 1);
        return cache[n];
    }

    idx_t c = n + 1;
    for (;;) {
        if (!is_prime_small((seq_t)c)) {      /* composite candidate */
            seq_t rem = mod_pow((seq_t)n, (uint64_t)c, (seq_t)c);
            if (rem == (seq_t)n % (seq_t)c) {
                cache[n] = (seq_t)c;
                return cache[n];
            }
        }
        ++c;
    }
}
