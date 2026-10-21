/*
 * A235157  Primes which have one or more occurrences of exactly five
 *          different digits.
 * https://oeis.org/A235157
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1068, u32 n <= 1574, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   digit_counts, ensure_capacity, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth prime that has exactly five distinct decimal digits. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Helper utilities from the builder */
#include "../utils/digit_counts.h"
#include "../utils/ensure_capacity.h"
#include "../utils/is_prime_big.h"

/* External interface: seq_t is the sequence element type, idx_t is the index type. */
seq_t a(idx_t n)
{
    /* Guard against empty or invalid index (behaviour below offset doesn't matter). */
    if (n == 0) return 0;

    /* Static cache state: values of the sequence found so far. */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static size_t cache_len  = 0;

    /* Last candidate tested; start just before the first odd prime (3). */
    static seq_t last_candidate = 1;

    /* Grow the sequence until we have at least n terms. */
    while ((size_t)n > cache_len)
    {
        last_candidate += 2;          /* only odd candidates */
        seq_t cand = last_candidate;

        /* Quick primality test.  The helper handles all sizes automatically. */
        if (!is_prime_big(cand))
            continue;

        /* Count distinct decimal digits in cand. */
        unsigned char cnt[10];
        digit_counts(cand, cnt);
        int distinct = 0;
        for (int d = 0; d < 10; ++d)
            if (cnt[d])
                ++distinct;

        if (distinct == 5)
        {
            /* Append to cache; grow if necessary. */
            ensure_capacity(cache_len + 1, &cache_cap,
                            (void **)&cache, sizeof(seq_t));
            cache[cache_len++] = cand;
        }
    }

    return cache[n - 1];
}
