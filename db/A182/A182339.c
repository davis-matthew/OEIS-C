/*
 * A182339  List of positive integers whose prime tower factorization, as
 *          defined in comments, contains the prime 2.
 * https://oeis.org/A182339
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth positive integer whose prime tower factorization
 *            contains prime 2. Computed by incremental search
 * Range:     u8 n <= 145, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/grow_seq.h"

static bool contains2_uint64(uint64_t n)
{
    if (n <= 1) return false;

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;

    factor_u64(n, primes, exps, &cnt);

    for (size_t i = 0; i < cnt; ++i) {
        if (primes[i] == 2) return true;
        if (exps[i] > 1 && contains2_uint64(exps[i])) return true;
    }
    return false;
}

/* External linkage as required */
seq_t a(idx_t n)
{
    /* static cache of computed terms of the sequence */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static idx_t next_check = 2;          /* start checking from 2 */

/* Initialise cache on first call */
    if (cache == NULL) {
        grow_seq(&cache, &cache_cap, 1);
        cache_len = 0;
    }

/* Extend the cache until we have enough terms */
    while (cache_len < (size_t)n) {
        if (contains2_uint64(next_check)) {
            grow_seq(&cache, &cache_cap, cache_len + 1);
            cache[cache_len] = (seq_t)next_check;
            ++cache_len;
        }
        ++next_check;
    }

/* Return the requested term (sequence is 1‑based) */
    return cache[n - 1];
}
