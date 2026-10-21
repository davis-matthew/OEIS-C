/*
 * A215203  a(0) = 0, a(n) = a(n - 1)*2^(n + 1) + 2^n - 1. That is, add one 0
 *          and n 1's to the binary representation of previous term.
 * https://oeis.org/A215203
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(0) = 0, a(n) = a(n-1)*2^(n+1) + 2^n - 1 (n >= 1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Helpers:   ensure_cache, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the big‑integer utilities supplied by the build environment. */

#include <stddef.h>
#include "../utils/ensure_cache.h"
#include "../utils/pow2.h"

/* Static cache for memoisation */
static seq_t *a_cache = NULL;      /* array of terms */
static size_t    a_cap   = 0;      /* capacity of a_cache */
static idx_t     a_last  = (idx_t)-1;   /* index of last computed term */

/* Return a(n) for any n >= 0, using memoisation.  The function is safe for
 * arbitrary ordering of calls and is efficient for n up to about 10,000. */
seq_t a(idx_t n)
{
    /* Ensure the cache can hold terms up to index n */
    ensure_cache(n + 1, &a_cache, &a_cap);

    /* Initialise cache[0] if not done yet */
    if (a_last == (idx_t)-1) {
        a_cache[0] = (seq_t)0;           /* a(0) = 0 */
        a_last = 0;
    }

    /* If the requested term is already computed, just return it */
    if (n <= a_last) {
        return a_cache[n];
    }

    /* Compute successive terms up to n */
    for (idx_t i = a_last + 1; i <= n; ++i) {
        /* a(i) = a(i-1)*2^(i+1) + 2^i - 1 */
        seq_t term = a_cache[i - 1] * pow2(i + 1) + pow2(i) - 1;
        a_cache[i] = term;
    }
    a_last = n;                       /* update last computed index */

    return a_cache[n];
}
