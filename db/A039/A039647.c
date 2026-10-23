/*
 * A039647  Related to A000032 (Lucas numbers): (n-1)!*L(n).
 * https://oeis.org/A039647
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n-1)! * L(n) (Lucas number L(1)=1, L(2)=3,
 *            L(n)=L(n-1)+L(n-2)) or equivalently: a(n) = (n-1) * ( a(n-1) +
 *            (n-2) * a(n-2) ) for n>2 initial values: a(1)=1, a(2)=3
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 21,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>          /* for size_t */
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

static seq_t *cache = NULL;          /* array of computed terms, index 1..n */
static idx_t  cache_cap = 0;         /* allocated size of cache (>= n+1) */
static idx_t  computed_until = 0;    /* highest n for which cache[n] is valid */

/* Return a(n) for the OEIS sequence A039647. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;          /* undefined for n<1, return 0 */

    /* Ensure cache has enough space for indices 0..n */
    if (cache == NULL) {
        cache = seq_alloc((size_t)n + 1);   /* index 0 unused */
        cache_cap = (idx_t)n + 1;
    } else if (cache_cap < (size_t)n + 1) {
        cache = seq_realloc(cache, cache_cap, (size_t)n + 1);
        cache_cap = (idx_t)n + 1;
    }

    /* Compute missing terms up to n */
    if (computed_until < n) {
        for (idx_t k = computed_until + 1; k <= n; ++k) {
            if (k == 1) {
                cache[1] = (seq_t)1;
            } else if (k == 2) {
                cache[2] = (seq_t)3;
            } else {
                seq_t term = (seq_t)(k - 1) *
                             ( cache[k - 1] + (seq_t)(k - 2) * cache[k - 2] );
                cache[k] = term;
            }
        }
        computed_until = n;
    }

    return cache[n];
}
