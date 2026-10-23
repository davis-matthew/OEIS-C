/*
 * A037486  Base 9 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,2.
 * https://oeis.org/A037486
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 9*a(n-1) + a(n-2) - 9*a(n-3); Initial values: a(1) = 1,
 *            a(2) = 11, a(3) = 100
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   grow_seq, mul9
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/grow_seq.h"
#include "../utils/mul9.h"

static seq_t *cache = NULL;          /* memoised values, cache[0] = a(1) */
static idx_t  cached_n = 0;          /* number of values currently in cache */
static size_t cache_cap = 0;         /* current capacity of cache */

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* out of range – behaviour doesn't matter */

    if (n <= cached_n)            /* already computed */
        return cache[n-1];

    /* Ensure sufficient capacity */
    if (cache_cap < (size_t)n)
        grow_seq(&cache, &cache_cap, (size_t)n);

    /* Compute new values up to n */
    for (idx_t i = cached_n + 1; i <= n; ++i) {
        seq_t val;
        if (i == 1)
            val = 1;
        else if (i == 2)
            val = 11;
        else if (i == 3)
            val = 100;
        else {
            seq_t t1 = mul9(cache[i-2]);   /* 9 * a(i-1) */
            seq_t t3 = mul9(cache[i-4]);   /* 9 * a(i-3) */
            val = t1 + cache[i-3] - t3;    /* a(i-2) + 9*a(i-1) - 9*a(i-3) */
        }
        cache[i-1] = val;
    }
    cached_n = n;
    return cache[n-1];
}
