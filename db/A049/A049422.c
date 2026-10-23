/*
 * A049422  Numbers k such that k^2 + 3 is prime.
 * https://oeis.org/A049422
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 36, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   grow_seq_array, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th integer k such that k^2 + 3 is prime
   (OEIS A049422).  The code searches for k in increasing order,
   tests primality of k^2+3 with the provided helper
   isPrimeGeneric, and keeps found k's in a static dynamic array. */

#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"
#include "../utils/grow_seq_array.h"

/* Return the n‑th element of the sequence, with 1‑based indexing. */
seq_t a(idx_t n)
{
    /* Static cache of the k-values.  cache_len stores how many
       terms have been found so far.  cache_cap is the allocated size. */
    static seq_t *cache   = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* The next k to test.  We start at k = 0, which produces
       the first sequence value 0. */
    static idx_t next_k = 0;

    /* Generate terms until we have at least n of them. */
    while (cache_len < n) {
        /* Only even k can give a prime k^2+3 (except the case k=1 gives 4,
           which is not prime), so we test only even k. */
        if ((next_k & 1ULL) == 0) {
            seq_t val = (seq_t)next_k * (seq_t)next_k + 3;
            if (isPrimeGeneric(val)) {
                /* Grow the cache if necessary. */
                if (cache_len == cache_cap) {
                    grow_seq_array(&cache, &cache_cap,
                                   cache_cap ? cache_cap * 2 : 16);
                }
                /* Store k itself, not the prime. */
                cache[cache_len++] = (seq_t)next_k;
            }
        }
        ++next_k;
    }

    /* n is 1‑based, so return the element at index n-1. */
    return cache[n - 1];
}
