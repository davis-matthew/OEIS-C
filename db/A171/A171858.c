/*
 * A171858  Number of n-step up-side self-avoiding walks on the lattice strip
 *          {0,1,2,3} x Z (up-side means that the walks move up and sideways
 *          but not down).
 * https://oeis.org/A171858
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 2*a(n-1) - a(n-3) + 2*a(n-4) + a(n-5)  (n>=5)
   with initial values a(0)=1, a(1)=2, a(2)=4, a(3)=9, a(4)=19 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/* Grow the cache to hold at least 'needed' elements.  It is
   safe for seq_t to be a POD or a trivial class.  Avoid
   out-of-bounds and initialize new slots to zero. */
static void ensure_capacity(idx_t needed, seq_t **cache, size_t *cap)
{
    if ((size_t)needed > *cap) {
        size_t new_cap = (*cap > 0) ? (*cap << 1) : 16;
        if (new_cap < (size_t)needed) new_cap = (size_t)needed;
        seq_t *tmp = (seq_t *)realloc(*cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        memset(tmp + *cap, 0, (new_cap - *cap) * sizeof(seq_t));
        *cache = tmp;
        *cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* static cache holds computed values */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t last = (idx_t)-1;      /* last index with a value */

    /* constant base values */
    static const seq_t base[5] = {
        (seq_t)1,
        (seq_t)2,
        (seq_t)4,
        (seq_t)9,
        (seq_t)19
    };

    /* initialise the cache with the first five terms only once */
    if (last == (idx_t)-1) {
        ensure_capacity(5, &cache, &cap);
        for (idx_t i = 0; i < 5; ++i)
            cache[i] = base[i];
        last = 4;
    }

    /* return the known value if it is already in the cache */
    if (n <= last)
        return cache[n];

    /* enlarge the cache if necessary */
    ensure_capacity((size_t)n + 1, &cache, &cap);

    /* compute missing values using the 5‑term recurrence */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t val = 2 * cache[i - 1];
        val -=      cache[i - 3];
        val += 2 * cache[i - 4];
        val +=      cache[i - 5];
        cache[i] = val;
    }
    last = n;
    return cache[n];
}
