/*
 * A127494  a(1)=2, a(2)=3; a(n)=a(n-2)+s^2, where s^2 is a minimal square
 *          such that a(n) is prime and is not already in the sequence.
 * https://oeis.org/A127494
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   is_prime_big, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(1)=2, a(2)=3;
 * a(n)=a(n-2)+s^2   where s^2 is the smallest square such that
 * a(n) is prime and not already in the sequence.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"
#include "../utils/seq_realloc.h"

static seq_t *seq_cache = NULL; /* dynamic array of computed terms */
static size_t cache_len = 0;    /* how many terms are currently stored */
static size_t cache_cap = 0;    /* allocated capacity of seq_cache */

/* Grow the cache so that it can store at least 'min_cap' elements. */
static void ensure_capacity(size_t min_cap)
{
    if (min_cap <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 4;
    while (new_cap < min_cap)
        new_cap <<= 1;
    seq_cache = seq_realloc(seq_cache, cache_cap, new_cap);
    cache_cap = new_cap;
}

/* Return true if 'cand' already appears in seq_cache[0 … limit-1]. */
static bool in_cache_before(seq_t cand, size_t limit)
{
    for (size_t j = 0; j < limit; ++j)
        if (cand == seq_cache[j])
            return true;
    return false;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0; /* undefined for n<1 */

    size_t target = (size_t)n; /* we need indices 0 … n-1 */

    /* Compute missing terms up to the requested index. */
    if (cache_len < target) {
        ensure_capacity(target);
        for (size_t i = cache_len; i < target; ++i) {
            seq_t val;
            if (i == 0) {
                val = (seq_t)2; /* a(1) */
            } else if (i == 1) {
                val = (seq_t)3; /* a(2) */
            } else {
                seq_t prev2 = seq_cache[i - 2];
                uint64_t s = 1;
                for (;;) {
                    seq_t sq = (seq_t)s * (seq_t)s;
                    seq_t cand = prev2 + sq;
                    if (is_prime_big(cand) && !in_cache_before(cand, i)) {
                        val = cand;
                        break;
                    }
                    ++s;
                }
            }
            seq_cache[i] = val;
        }
        cache_len = target;
    }

    return seq_cache[n - 1];
}
