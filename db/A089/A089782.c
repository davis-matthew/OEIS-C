/*
 * A089782  Smallest prime of the form prime(k) concatenated with prime(k+n).
 * https://oeis.org/A089782
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = smallest prime of the form prime(k) concatenated with
 *            prime(k+n)
 * Range:     u8 n <= 2, u16 n <= 17, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   concat_parts, get_prime, is_prime_big, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#include "../utils/get_prime.h"
#include "../utils/concat_parts.h"
#include "../utils/is_prime_big.h"
#include "../utils/seq_realloc.h"

/* Sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n < 1, offset is 1 */

    /* Static cache for memoisation */
    static seq_t *cache   = NULL;
    static size_t cache_cap = 0;

    /* Resize cache if necessary */
    if ((size_t)n > cache_cap) {
        seq_t *new_cache = seq_realloc(cache, cache_cap, (size_t)n);
        if (!new_cache) abort();   /* allocation failure */
        /* initialise newly added entries to 0 (unknown) */
        for (size_t i = cache_cap; i < (size_t)n; ++i) new_cache[i] = 0;
        cache = new_cache;
        cache_cap = (size_t)n;
    }

    /* Return cached value if present (non‑zero is a valid prime) */
    seq_t *slot = &cache[(size_t)(n - 1)];
    if (*slot != 0) return *slot;

    /* Search for the smallest k such that prime(k) concatenated with prime(k+n) is prime */
    seq_t result = 0;
    for (idx_t k = 1; ; ++k) {
        uint32_t p1 = get_prime(k);
        uint32_t p2 = get_prime(k + n);
        seq_t candidate = concat_parts((idx_t)p1, (idx_t)p2);
        if (is_prime_big(candidate)) {
            result = candidate;
            break;
        }
    }

    *slot = result;    /* cache the result */
    return result;
}
