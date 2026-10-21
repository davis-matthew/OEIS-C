/*
 * A234117  Number of (n+1) X (3+1) 0..2 arrays with every 2 X 2 subblock
 *          having the absolute values of all six edge and diagonal
 *          differences no larger than 1.
 * https://oeis.org/A234117
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 20*a(n-1) - 10*a(n-2) - 324*a(n-3) - 277*a(n-4) +
 *            144*a(n-5) Initial values: a(1)=673, a(2)=12593, a(3)=231713,
 *            a(4)=4279065, a(5)=79003521
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 13, u128 n <= 29,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_cache.h"

seq_t a(idx_t n)
{
    static const seq_t base[5] = { 673ULL, 12593ULL, 231713ULL, 4279065ULL, 79003521ULL };
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t computed = 0;   /* largest n for which cache[n-1] is valid */

    if (n <= 5)
        return base[n - 1];

    /* Ensure capacity for all needed indices (1‑based) */
    ensure_cache(n, &cache, &cap);

    /* Initialise first five terms if this is the first use of the cache */
    if (computed == 0) {
        for (idx_t i = 0; i < 5; ++i)
            cache[i] = base[i];
        computed = 5;
    }

    /* Compute missing terms up to n using the linear recurrence */
    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t val = (seq_t)20 * cache[i - 2]
                  - (seq_t)10 * cache[i - 3]
                  - (seq_t)324 * cache[i - 4]
                  - (seq_t)277 * cache[i - 5]
                  + (seq_t)144 * cache[i - 6];
        cache[i - 1] = val;
    }
    computed = n;

    return cache[n - 1];
}
