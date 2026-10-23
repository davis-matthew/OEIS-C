/*
 * A121326  Primes of the form 4*k^2 + 1.
 * https://oeis.org/A121326
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 41, u32 n <= 4614, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   append_candidate, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Seq: A121326 – Primes of the form 4*k^2 + 1
 * Implementation: iterate over k, test 4*k^2+1 for primality,
 * cache results so that calls in any order and for any n are fast.
 */
#include <stdbool.h>
#include <stddef.h>
#include "../utils/isPrimeGeneric.h"
#include "../utils/append_candidate.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t next_k = 1;           /* next k to test */

    if (n == 0)
        return 0;                      /* undefined for offset 0, harmless */

    /* extend the cache until it contains at least n terms */
    while (cache_len < n) {
        seq_t val = ((seq_t)next_k * (seq_t)next_k) * 4 + 1;
        if (isPrimeGeneric(val))
            append_candidate(&cache, &cache_len, &cache_cap, val);
        ++next_k;
    }
    return cache[n - 1];
}
