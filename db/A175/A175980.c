/*
 * A175980  a(1) = 1, for n >= 2: a(n) = a(n-1)^n + 1.
 * https://oeis.org/A175980
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)^n + 1, a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 4, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   big_pow, seq_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/seq_one.h"
#include "../utils/big_pow.h"

static seq_t *cache = NULL;      /* array of pre‑computed values */
static size_t cache_cap = 0;     /* allocated capacity (>= n+1) */
static size_t cache_len = 0;     /* number of values already filled */

seq_t a(idx_t n)
{
    if (n < 1)
        return 0; /* undefined for n < 1 */

    /* grow the cache if necessary */
    if (!cache || cache_cap < (size_t)n + 1) {
        seq_t *new_cache = (seq_t*)realloc(cache,
                                           ((size_t)n + 1) * sizeof(seq_t));
        cache = new_cache;
        cache_cap = (size_t)n + 1;
    }

    /* compute any missing terms up to n */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t val;
        if (i == 1) {
            val = seq_one();                     /* a(1) = 1 */
        } else {
            seq_t base  = cache[(size_t)i - 1];   /* a(i‑1) */
            seq_t powed = big_pow(base, (seq_t)i);/* base^i   */
            val = powed + 1;                     /* a(i) = base^i + 1 */
        }
        cache[(size_t)i] = val;
    }

    cache_len = (size_t)n;
    return cache[(size_t)n];
}
