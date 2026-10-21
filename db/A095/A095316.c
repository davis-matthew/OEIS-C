/*
 * A095316  Primes in whose binary expansion the number of 1-bits is > number
 *          of 0-bits minus 2.
 * https://oeis.org/A095316
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 51, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   bitlen_seq_t, is_prime_big, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence consists of those primes p whose binary expansion satisfies
   popcount(p) > (bitlen(p) - popcount(p) - 2).  For a prime p with
   bitlen(p)=len and popcount(p)=pop this is equivalent to
   2*pop > len-2  <=>  2*pop >= len-1.  */
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/seq_realloc.h"
#include "../utils/bitlen_seq_t.h"

seq_t a(idx_t n) {
    static seq_t *cache = NULL;      /* array of accepted primes */
    static size_t cache_cap = 0;     /* allocated capacity */
    static size_t cache_len = 0;     /* number of stored primes */
    static seq_t last_cand = 1;      /* last candidate examined */

    /* Extend the cache until it contains at least n elements. */
    while (cache_len < n) {
        seq_t cand = last_cand + 1;          /* next integer to test */
        last_cand = cand;                    /* remember it for the next call */

        if (!is_prime_big(cand))
            continue;                       /* skip non‑primes */

        /* Compute popcount. */
        seq_t x = cand;
        seq_t pop = 0;
        while (x) {
            pop += (x & 1);
            x >>= 1;
        }

        /* Length of the binary representation. */
        unsigned len = bitlen_seq_t(cand);

        /* Check the inequality 2*pop >= len-1. */
        if ((pop << 1) >= (seq_t)(len - 1)) {
            /* Grow the cache if necessary. */
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
                seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                if (!tmp) abort();          /* out of memory */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = cand;
        }
    }

    return cache[n - 1];
}
