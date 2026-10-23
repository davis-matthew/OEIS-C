/*
 * A120179  a(1)=2; a(n)=floor((13+sum(a(1) to a(n-1)))/6).
 * https://oeis.org/A120179
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((13 + sum_{k=1}^{n-1} a(k)) / 6), a(1)=2
 * Range:     u8 n <= 22, u16 n <= 57, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cache for values and prefix sums.  All indices start at 1. */
static seq_t *a_cache   = NULL; /* a(n) values */
static seq_t *sum_cache = NULL; /* cumulative sums, sum_cache[0]=0 */
static idx_t cache_size = 0;    /* largest n currently cached, 0 means none */

/* Ensure space and compute terms up to 'need'. */
static void
ensure_cache(idx_t need)
{
    if (need <= cache_size)
        return;                 /* already ready */

    /* Allocate (or reallocate) arrays of size need+1 (index 0 not used for a). */
    size_t new_cap = (size_t)(need + 1);
    if (!a_cache) {
        a_cache   = (seq_t *)malloc(new_cap * sizeof(seq_t));
        sum_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!a_cache || !sum_cache)
            return; /* memory failure; caller will result in 0 */
        sum_cache[0] = (seq_t)0; /* sum of zero terms */
    } else {
        a_cache   = (seq_t *)realloc(a_cache,   new_cap * sizeof(seq_t));
        sum_cache = (seq_t *)realloc(sum_cache, new_cap * sizeof(seq_t));
        if (!a_cache || !sum_cache)
            return; /* memory failure */
    }

    /* Compute new terms from cache_size+1 up to 'need'. */
    for (idx_t i = cache_size + 1; i <= need; ++i) {
        seq_t val = (sum_cache[i - 1] + (seq_t)13) / (seq_t)6;
        a_cache[i] = val;
        sum_cache[i] = sum_cache[i - 1] + val;
    }
    cache_size = need;
}

/* Sequence function required by the harness */
seq_t
a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;        /* out of range; not used by tests */

    ensure_cache(n);
    return a_cache[n];
}
