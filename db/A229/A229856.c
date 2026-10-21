/*
 * A229856  Primes of the form 384*k + 257.
 * https://oeis.org/A229856
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   primes of the form 384*k + 257, k ≥ 0, offset 1
 * Range:     u8 none, u16 n <= 38, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   grow_seq_array, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function returns the nth such prime.  It keeps a static
 * cache of previously found primes, extending the cache only as
 * required.  Candidates are checked with the generic 64‑bit prime
 * test (is_prime).  For arbitrary‑precision builds the numbers
 * involved are still small enough to fit in a 64‑bit unsigned
 * integer, so the same test is adequate. */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#include "../utils/is_prime.h"
#include "../utils/grow_seq_array.h"

/*                                  */
seq_t a(idx_t n)
{
    /* a(1) is the first term (257).  For n < 1 the result is
     * undefined, but returning 0 keeps the behaviour benign. */
    if (n < 1) return 0;

    /* static cache of discovered primes */
    static seq_t *cache      = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t  next_k     = 0;      /* next k to test (k=0 → 257) */

    /* initialise cache with the first prime 257 */
    if (cache_len == 0) {
        grow_seq_array(&cache, &cache_cap, 1);
        cache[0] = (seq_t)257u;
        cache_len = 1;
        next_k = 1;                    /* start with k = 1 → 641 */
    }

    /* generate primes until we have at least n terms */
    while (cache_len < n) {
        uint64_t cand64 = 257ULL + 384ULL * (uint64_t)next_k;
        seq_t cand     = (seq_t)cand64;

        /* the generic 64‑bit prime test is sufficient */
        if (is_prime((idx_t)cand)) {
            grow_seq_array(&cache, &cache_cap, cache_len + 1);
            cache[cache_len++] = cand;
        }
        ++next_k;
    }

    return cache[n - 1];
}
