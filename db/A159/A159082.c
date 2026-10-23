/*
 * A159082  Numbers whose squares added to 7! are prime.
 * https://oeis.org/A159082
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is the nth positive integer a such that 7! + a^2 is prime
 * Range:     u8 none, u16 n <= 17, u32 n <= 19, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   grow_seq, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/grow_seq.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Static cache of discovered sequence values. */
    static seq_t *cache      = NULL;   /* array of a(k) */
    static size_t cache_cap = 0;       /* current allocation size */
    static size_t cache_sz  = 0;       /* number of values currently stored */
    static uint64_t next_candidate = 1;/* last candidate examined (starts at 1) */

    /* Extend cache if necessary. */
    if (cache_sz + 1 > cache_cap)
        grow_seq(&cache, &cache_cap, cache_sz + 1);

    /* Generate additional terms until reaching index n. */
    while (cache_sz < n) {
        ++next_candidate; /* consider the next integer candidate */

        /* Skip obvious composites: a must not be divisible by 2,3,5,7. */
        if ((next_candidate & 1ULL) == 0ULL ||
            (next_candidate % 3ULL) == 0ULL ||
            (next_candidate % 5ULL) == 0ULL ||
            (next_candidate % 7ULL) == 0ULL)
            continue;

        /* Compute 7! + a^2 using the generic seq_t type. */
        seq_t cand = (seq_t)next_candidate;
        seq_t p = (seq_t)5040 + cand * cand;

        /* Check primality of the resulting number. */
        if (is_prime_big(p)) {
            /* Store the new term in the cache. */
            if (cache_sz + 1 > cache_cap)
                grow_seq(&cache, &cache_cap, cache_sz + 1);
            cache[cache_sz++] = cand;
        }
    }

    return cache[n - 1];
}
