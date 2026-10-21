/*
 * A238216  Starting with a(1) = 3, a(2) = 5, a(n+1) is the smallest prime
 *          number greater than the previous term a(n) such that there exists
 *          k satisfying 1<=k<n, a(n+1) = 2*a(n) + a(k).
 * https://oeis.org/A238216
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute OEIS A238216
 *
 * a(1) = 3, a(2) = 5.
 * For n ≥ 2: a(n+1) is the smallest prime greater than a(n)
 * such that ∃k, 1≤k<n, with a(n+1) = 2*a(n) + a(k).
 * The sequence is finite.  For indices beyond its length,
 * the function returns 0.
 *
 * The implementation memoises the sequence in a static array
 * (capacity 10000, sufficient for the required tests).  Each
 * call generates missing terms on demand, so calls may arrive
 * in any order.  Prime testing uses the provided is_prime_big
 * routine (which works for all supported seq_t types).
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"

#define MAX_TERMS 10000  /* enough for all requested n ≤ 10000 */

/* Static cache for memoisation */
static seq_t cache[MAX_TERMS];
static size_t cache_len = 0;

/* Helper: initialise the cache with the first two terms */
static inline void init_cache(void)
{
    if (cache_len == 0) {
        cache[0] = (seq_t)3;
        cache[1] = (seq_t)5;
        cache_len = 2;
    }
}

/* Helper: generate the next term if possible.
 * Returns true on success, false if the sequence terminates
 * (no qualifying prime) or the cache is full. */
static bool generate_next(void)
{
    if (cache_len < 2 || cache_len >= MAX_TERMS)
        return false;

    seq_t cur     = cache[cache_len - 1];
    seq_t two_cur = cur + cur;

    bool  found = false;
    seq_t best  = 0;

    for (size_t i = 0; i < cache_len - 1; ++i) {
        seq_t val = two_cur + cache[i];
        if (val <= cur)          /* must be greater than a(n) */
            continue;
        if (is_prime_big(val)) {
            if (!found || val < best) {
                best  = val;
                found = true;
            }
        }
    }

    if (!found)
        return false;            /* sequence ends at current length */

    cache[cache_len++] = best;
    return true;
}

/* Public sequence accessor */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for n<1 */

    /* initialise cache on first call */
    if (cache_len == 0)
        init_cache();

    /* generate terms up to requested index (1‑based) */
    while (cache_len < (size_t)n) {
        if (!generate_next())
            break;           /* sequence finite – cannot extend further */
    }

    if (cache_len >= (size_t)n)
        return cache[(size_t)n - 1];
    else
        return (seq_t)0;   /* index beyond finite sequence */
}
