/*
 * A060706  For n >= 1 a(n) is the size of the conjugacy class in the
 *          symmetric group S_(4n) consisting of permutations whose cycle
 *          decomposition is a product of n disjoint 4-cycles.
 * https://oeis.org/A060706
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (4n)! / (n! * 4^n) Recurrence: a(0) = 1, a(n) = a(n−1) *
 *            (4n−1) * (4n−2) * (4n−3) for n ≥ 1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
/* idx_t and seq_t are defined by the build system. */

/* Cache for already computed terms.  cache[i] == a(i).  The cache grows
   lazily on demand and is shared between calls. */
static seq_t *cache = NULL;
static idx_t cached_max = 0;           /* highest index stored in cache */

/* Ensure the cache can hold terms up to 'n' (inclusive).  The cache is
   reallocated if necessary; new entries are not initialized. */
static void ensure_cache(idx_t n)
{
    if (n <= cached_max) return;                  /* already covered */
    size_t new_len = (size_t)(n + 1);             /* one past the max index */
    if (!cache)
        cache = (seq_t *)malloc(new_len * sizeof(seq_t));
    else
        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    /* initialize a(0) */
    if (cached_max == 0 && cache[0] == 0) cache[0] = (seq_t)1;
    /* compute missing terms */
    for (idx_t i = cached_max + 1; i <= n; ++i) {
        seq_t f1 = (seq_t)(4 * i - 1);
        seq_t f2 = (seq_t)(4 * i - 2);
        seq_t f3 = (seq_t)(4 * i - 3);
        seq_t res = cache[i - 1] * f1;
        res *= f2;
        res *= f3;
        cache[i] = res;
    }
    cached_max = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    ensure_cache(n);
    return cache[n];
}
