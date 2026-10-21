/*
 * A209121  Recursive sequence: a(n) = 1+mod(abs(n-2+a(n-2)),n) +
 *          mod(abs(n-1+a(n-1)),n-1), a(0) = 1, a(1) = 1, a(2) = 1, a(3)=1,
 *          a(4)=2.
 * https://oeis.org/A209121
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + (n-2+a(n-2) mod n) + (n-1+a(n-1) mod (n-1)) Initial
 *            values: a(0)=1, a(1)=1, a(2)=1, a(3)=1, a(4)=2
 * Range:     u8 n <= 169, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/ensure_cache.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;

    /* Initialise cache with first five terms */
    if (cache == NULL) {
        /* ensure_cache(n) guarantees size for indices 0..n inclusive   */
        ensure_cache(4, &cache, &cap);     /* sufficient for indices 0..4   */
        cache[0] = 1; cache[1] = 1; cache[2] = 1;
        cache[3] = 1; cache[4] = 2;
    }

    if (n < 5)
        return cache[n];

    /* Make sure cache large enough for 'n' */
    ensure_cache(n, &cache, &cap);

    /* Return already computed value */
    if (cache[n] != 0)
        return cache[n];

    seq_t prev2 = a(n-2);
    seq_t prev1 = a(n-1);

    /* All terms in the recurrence are non‑negative for n >= 5 */
    seq_t part1 = (seq_t)n - 2 + prev2;
    seq_t part2 = (seq_t)n - 1 + prev1;

    seq_t mod1 = part1 % (seq_t)n;
    seq_t mod2 = part2 % (seq_t)(n-1);

    seq_t result = (seq_t)1 + mod1 + mod2;

    cache[n] = result;
    return result;
}
