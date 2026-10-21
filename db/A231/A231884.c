/*
 * A231884  Number of maximal 2-independent sets in the 3-dimensional (2, 2,
 *          n) grid graph.
 * https://oeis.org/A231884
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-2)+4*a(n-3)+4*a(n-4)-9*a(n-6)-3*a(n-7); Initial
 *            values:
 *            a(0)=0,a(1)=4,a(2)=4,a(3)=20,a(4)=32,a(5)=80,a(6)=180,a(7)=408
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 26, u64 n <= 53, u128 n <= 107,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free.h"

static seq_t *cache = NULL;          /* array holding the values */
static idx_t   max_computed = 0;     /* highest index already computed */
static idx_t   capacity   = 0;       /* current storage capacity */

/* initialise cache with the eight known base terms */
static void init_cache(void)
{
    if (cache != NULL)
        return;
    capacity = 8;                                /* indices 0..7 */
    cache = seq_alloc_new_array(capacity);
    cache[0] = 0;
    cache[1] = 4;
    cache[2] = 4;
    cache[3] = 20;
    cache[4] = 32;
    cache[5] = 80;
    cache[6] = 180;
    cache[7] = 408;
    max_computed = 7;
}

/* enlarge the allocated array if n+1 exceeds its current capacity */
static void ensure_capacity(idx_t n)
{
    if (n < capacity)
        return;
    idx_t new_cap = n + 1;
    if (new_cap < capacity * 2)
        new_cap = capacity * 2;
    seq_t *new_buf = seq_alloc_new_array(new_cap);
    for (idx_t i = 0; i < capacity; ++i)
        new_buf[i] = cache[i];
    for (idx_t i = capacity; i < new_cap; ++i)
        new_buf[i] = 0;
    seq_free(cache);
    cache = new_buf;
    capacity = new_cap;
}

/* compute all terms from max_computed+1 up to n (inclusive) */
static void compute_up_to(idx_t n)
{
    for (idx_t i = max_computed + 1; i <= n; ++i)
    {
        if (i <= 7)          /* base values already set */
            continue;
        seq_t term =
              3 * cache[i - 2] + 4 * cache[i - 3] + 4 * cache[i - 4];
        term -= 9 * cache[i - 6];
        term -= 3 * cache[i - 7];
        cache[i] = term;
    }
    max_computed = n;
}

/* Public sequence accessor */
seq_t a(idx_t n)
{
    init_cache();
    if (n > max_computed) {
        ensure_capacity(n);
        compute_up_to(n);
    }
    return cache[n];
}
