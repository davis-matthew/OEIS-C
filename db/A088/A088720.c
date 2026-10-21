/*
 * A088720  Unique monotone sequence satisfying a(a(a(n))) = 2n.
 * https://oeis.org/A088720
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is defined for n ≥ 3 by the recurrence * * a(2n) = 2 * a(n)
 *            * a(4n+1) = a(2n+1) + 2 * a(n) * a(4n+3) = 3 * a(2n+1) – 2 *
 *            a(n) * * Base values are * a(3)=4, a(4)=5, a(5)=6, a(6)=8,
 *            a(7)=9, a(8)=10, * a(9)=11, a(10)=12, a(11)=14 * * The function
 *            below memoises these values so that any * call t...
 * Range:     u8 n <= 191, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   compute_a
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/compute_a.h"

static seq_t *cache = NULL;     /* memoisation array */
static idx_t cache_size = 0;    /* number of allocated slots */

/* Ensure that cache can hold indices up to n. */
static void
ensure_cache(idx_t n)
{
    if (n + 1 <= cache_size)
        return;

    /* Grow the cache exponentially for amortised linear time. */
    idx_t new_size = cache_size ? cache_size * 2 : 16;
    while (new_size <= n)
        new_size <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_size * sizeof(seq_t));
    if (!tmp) {
        /* Allocation failed – the caller will crash or return 0. */
        return;
    }
    /* Initialise newly allocated slots to 0. */
    for (idx_t i = cache_size; i < new_size; ++i)
        tmp[i] = 0;
    cache = tmp;
    cache_size = new_size;
}

/* Forward declaration of the recursive helper. */
static seq_t compute_a(idx_t n);

/* Public entry point.  For n < 3 the value is unspecified; we return 0. */
seq_t a(idx_t n)
{
    if (n < 3)
        return 0;

    ensure_cache(n);
    if (cache[n] != 0)
        return cache[n];

    cache[n] = compute_a(n);
    return cache[n];
}
