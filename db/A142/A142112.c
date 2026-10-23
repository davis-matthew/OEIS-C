/*
 * A142112  Primes congruent to 2 mod 37.
 * https://oeis.org/A142112
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   primes equal to 2 or congruent to 39 mod 74 (i.e., primes ≡2
 *            (mod 37))
 * Range:     u8 n <= 2, u16 n <= 178, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   grow_seq, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The code lazily generates terms and caches them for future calls. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/grow_seq.h"
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Persistent cache of computed terms. */
    static seq_t *cache   = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    /* The arithmetic progression index (m) for values 2 + 37*m.  */
    static idx_t next_m = 0;

    /* Initialise the cache with the first known term 2. */
    if (cache_len == 0) {
        grow_seq(&cache, &cache_cap, 1);
        cache[0] = (seq_t)2;
        cache_len = 1;
        next_m = 1;           /* next candidate will be 2 + 37*1 = 39 */
    }

    /* Generate terms until we reach the requested index. */
    while (cache_len < n) {
        idx_t cand = 2 + (idx_t)37 * next_m;
        if (is_prime(cand)) {
            grow_seq(&cache, &cache_cap, cache_len + 1);
            cache[cache_len++] = (seq_t)cand;
        }
        next_m++;
    }

    return cache[n - 1];
}
