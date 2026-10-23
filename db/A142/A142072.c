/*
 * A142072  Primes congruent to 19 mod 34.
 * https://oeis.org/A142072
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 406, u32 n <= 295, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   grow_seq, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Compute the n-th prime that is congruent to 19 modulo 34.
 * Uses a simple incremental search and memoisation.
 * The function is thread‑safe only if called from a single thread.
 * Suitable for int, 64‑bit, or arbitrary‑precision seq_t.
 */
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/grow_seq.h"

seq_t a(idx_t n)
{
    /* trivial case: below offset (n < 1) */
    if (n < 1)
        return 0;

    /* static cache of found primes */
    static seq_t *cache = NULL;
    static size_t sz = 0;      /* number of stored terms */
    static size_t cap = 0;     /* allocated capacity */

    /* candidate value of the next check (always 19 mod 34) */
    static seq_t candidate = 19;
    static bool initialized = false;

    /* initialise candidate on first call */
    if (!initialized)
    {
        candidate = 19;
        initialized = true;
    }

    /* return from cache if already computed */
    if (n <= sz)
        return cache[n - 1];

    /* expand cache until we have at least n terms */
    while (sz < n)
    {
        /* find next prime of form 19 mod 34 */
        while (!is_prime_big(candidate))
            candidate += 34;

        /* grow cache if necessary */
        if (sz == cap)
            grow_seq(&cache, &cap, sz + 1);

        /* store prime */
        cache[sz++] = candidate;

        /* move to next candidate (19 mod 34) */
        candidate += 34;
    }

    return cache[n - 1];
}
