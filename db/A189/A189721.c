/*
 * A189721  Partial sums of A189718.
 * https://oeis.org/A189721
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS formula for A189721 (partial sums of A189718):
 *   s(0) = 0,  s(1) = 0,
 *   s(n) = s([(n+2)/3]) + [(n+1)/3] - s([(n+1)/3]) + [n/3] - s([n/3])
 *   for n >= 2.
 *
 * The function `a` implements this recurrence with memoisation.
 */
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Forward declaration of the static recursive routine. */
static seq_t _seq_a_computed(idx_t);

/* Dynamic cache for computed values.
 *  cache[n] holds s(n) for 0 <= n < cache_cap, and flag[n] indicates
 *  whether cache[n] contains a valid value. */
static seq_t *cache = 0;
static bool *flag  = 0;
static idx_t cache_cap = 0;

/* Ensure that the cache has room for indices up to n. */
static void _ensure_cache(idx_t n)
{
    if (n + 1 <= cache_cap)
        return;

    idx_t new_cap = n + 1;

    seq_t *new_cache = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
    if (!new_cache)
        abort();

    bool *new_flag  = (bool *)realloc(flag,  (size_t)new_cap * sizeof(bool));
    if (!new_flag)
        abort();

    size_t added = (size_t)(new_cap - cache_cap);
    /* Initialise new entries to zero / false. */
    (void)memset(new_cache + cache_cap, 0, added * sizeof(seq_t));
    (void)memset(new_flag  + cache_cap, 0, added * sizeof(bool));

    cache = new_cache;
    flag  = new_flag;
    cache_cap = new_cap;
}

/* Recursive computation with memoisation. */
static seq_t _seq_a_computed(idx_t n)
{
    if (n <= 1)          /* s(0)=0, s(1)=0 */
        return 0;

    if (flag[n])         /* already computed */
        return cache[n];

    /* Compute indices needed for the recurrence. */
    idx_t n3 = n / 3;
    idx_t n1 = (n + 1) / 3;
    idx_t n2 = (n + 2) / 3;

    seq_t val = _seq_a_computed(n2) + (seq_t)n1
                - _seq_a_computed(n1)
                + (seq_t)n3
                - _seq_a_computed(n3);

    cache[n] = val;
    flag[n]  = true;
    return val;
}

/* Public API. */
seq_t a(idx_t n)
{
    _ensure_cache(n);
    return _seq_a_computed(n);
}
