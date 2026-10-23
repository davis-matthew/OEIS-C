/*
 * A142084  Primes congruent to 12 mod 35.
 * https://oeis.org/A142084
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 271, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   grow_seq, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for primes congruent to 12 mod 35, returning the nth such prime */
#include "../utils/isPrimeGeneric.h"
#include "../utils/grow_seq.h"
#include <stddef.h> /* for size_t */

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* static cache of found primes */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_size = 0;

    /* next candidate index k, starting at 1 => candidate 35*1+12 = 47 */
    static idx_t next_k = 1;

    /* generate primes until we have at least n terms */
    while (cache_size < (size_t)n) {
        seq_t cand = (seq_t)35 * next_k + 12;  /* candidate prime */

        if (isPrimeGeneric(cand)) {
            /* ensure capacity for cache */
            grow_seq(&cache, &cache_cap, cache_size + 1);
            cache[cache_size++] = cand;         /* store found prime */
        }
        ++next_k;                               /* move to next candidate */
    }

    return cache[n - 1];
}
