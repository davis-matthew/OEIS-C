/*
 * A142410  Primes congruent to 37 mod 48.
 * https://oeis.org/A142410
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 309, u32 n <= 320, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   grow_seq, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Result: the n-th prime congruent to 37 (mod 48) (OEIS A142410) */
/* a(1) = 37, a(2) = 181, a(3) = 229, ... */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/grow_seq.h"

seq_t a(idx_t n)
{
    /* Return 0 for indices below the offset – behaviour there is irrelevant. */
    if (n == 0) return 0ULL;

    /* Static cache of already found terms. */
    static seq_t *cache     = NULL;
    static idx_t  count     = 0;   /* number of terms currently cached  */
    static size_t cap       = 0;   /* allocated capacity of cache      */
    static idx_t next_val   = 37;  /* next integer ≡ 37 (mod 48)       */

    /* Grow the cache and generate terms until we have at least n of them. */
    while (count < n)
    {
        /* Ensure we have space for the next potential entry. */
        if (count == cap)
            grow_seq(&cache, &cap, count + 1);

        /* Test whether the current candidate is prime. */
        if (is_prime_big((seq_t)next_val))
        {
            cache[count++] = (seq_t)next_val;
        }

        /* Advance to the next integer ≡ 37 (mod 48). */
        next_val += 48;
    }

    return cache[n - 1];
}
