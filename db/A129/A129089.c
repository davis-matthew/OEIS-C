/*
 * A129089  a(0)=a(1)=1. a(n) = gcd(a(n-1), n) + gcd(a(n-2), n).
 * https://oeis.org/A129089
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(a(n-1), n) + gcd(a(n-2), n); a(0) = a(1) = 1
 * Range:     u8 n <= 621, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   ensure_cache, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/gcd.h"
#include "../utils/ensure_cache.h"

static seq_t *cache = NULL;
static size_t cap = 0;
static idx_t max_comp = 1;  /* indices 0 and 1 are known */

/* Return a(n) for all n >= 0, memoising results.  The function can be
   called in any order; previously computed values are reused. */
seq_t a(idx_t n)
{
    if (n <= 1)
        return 1;               /* a(0) = a(1) = 1 */

    /* Make sure the cache can hold terms up to n. */
    ensure_cache(n, &cache, &cap);

    /* Initialise the first two terms if they are not already set. */
    if (cache[0] == 0)
        cache[0] = 1;
    if (cache[1] == 0)
        cache[1] = 1;

    /* Compute any terms that have not yet been evaluated. */
    if (n > max_comp)
    {
        /* max_comp is at least 1 here (first two terms are ready). */
        for (idx_t i = max_comp + 1; i <= n; ++i)
        {
            seq_t g1 = gcd(cache[i-1], (seq_t)i);
            seq_t g2 = gcd(cache[i-2], (seq_t)i);
            cache[i] = g1 + g2;
        }
        max_comp = n;
    }

    return cache[n];
}
