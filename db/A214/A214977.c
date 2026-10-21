/*
 * A214977  Number of terms in Lucas representations of 1,2,...,n.
 * https://oeis.org/A214977
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=1..n} f(k) where f(k) is the number of Lucas
 *            numbers used in the greedy representation of k
 * Range:     u8 n <= 99, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   alloc_seq_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/seq_free_array.h"

static seq_t *cache = NULL;            /* cumulative sums cache */
static idx_t cache_cap = 0;            /* cache size (indices 0..cap-1) */

static seq_t *lucas = NULL;            /* array of Lucas numbers */
static idx_t lucas_len = 0;           /* number of Lucas numbers stored */
static idx_t lucas_cap = 0;           /* allocated capacity of lucas */

/* Ensure that lucas array contains all Lucas numbers up to at least n. */
static void
ensure_lucas(idx_t n)
{
    if (lucas_len == 0) {
        /* initialise with the first two Lucas numbers */
        lucas_cap = 8;
        lucas     = alloc_seq_array(lucas_cap);
        lucas[0] = (seq_t)2;
        lucas[1] = (seq_t)1;
        lucas_len = 2;
    }

    while (lucas[lucas_len - 1] <= (seq_t)n) {
        /* double storage if needed */
        if (lucas_len == lucas_cap) {
            lucas_cap <<= 1;                 /* grow by factor 2 */
            seq_t *tmp = alloc_seq_array(lucas_cap);
            for (idx_t i = 0; i < lucas_len; ++i)
                tmp[i] = lucas[i];
            seq_free_array(lucas);
            lucas = tmp;
        }
        /* append next Lucas number */
        seq_t next = lucas[lucas_len - 1] + lucas[lucas_len - 2];
        lucas[lucas_len++] = next;
    }
}

/* Greedy count of Lucas terms needed to represent k. */
static seq_t
count_terms(idx_t k)
{
    seq_t rem = (seq_t)k;
    seq_t cnt = 0;
    for (idx_t i = lucas_len; i-- > 0;) {
        if (lucas[i] <= rem) {
            rem -= lucas[i];
            ++cnt;
        }
    }
    return cnt;
}

/* public function: a(n) */
seq_t
a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* out of sequence range, return 0 */

    /* Make sure the Lucas numbers cover the range. */
    ensure_lucas(n);

    /* If we already have values up to n, return it. */
    if (cache_cap > n) {
        return cache[n];
    }

    /* Extend the cache to include n */
    idx_t old_cap = cache_cap;
    idx_t new_cap = n + 1;          /* indices 0..n */

    seq_t *new_cache = alloc_seq_array(new_cap);

    /* copy existing data */
    for (idx_t i = 0; i < old_cap; ++i)
        new_cache[i] = cache[i];
    new_cache[0] = (seq_t)0;        /* a(0) is unused but set to 0 */

    seq_t sum = (old_cap == 0) ? (seq_t)0 : cache[old_cap - 1];

    /* compute new cumulative sums from old_cap up to n */
    for (idx_t k = old_cap; k <= n; ++k) {
        seq_t t = count_terms(k);
        sum += t;
        new_cache[k] = sum;
    }

    /* replace old cache */
    if (cache)
        seq_free_array(cache);
    cache = new_cache;
    cache_cap = new_cap;

    return cache[n];
}
