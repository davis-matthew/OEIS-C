/*
 * A127594  Numbers k such that 256 k + 85 is prime.
 * https://oeis.org/A127594
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers k such that 256*k+85 is prime. */
/* a(n) is the nth such integer (offset = 1). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    static seq_t *cache      = NULL;   /* array storing found k-values   */
    static size_t cache_cap  = 0;      /* allocated capacity               */
    static size_t computed   = 0;      /* number of terms already stored  */
    static idx_t  candidate  = 0;      /* next integer k to test          */

    if (n == 0)            /* undefined, but return 0 as a fall‑back   */
        return (seq_t)0;

    /* grow the cache until we have at least n terms */
    while (computed < (size_t)n)
    {
        /* search for the next k whose 256*k+85 is prime */
        for (;; ++candidate)
        {
            seq_t m = (seq_t)256 * candidate + (seq_t)85; /* 256k+85 */

            if (is_prime_small((uint64_t)m))
            {
                /* ensure enough space in the cache array */
                if (computed >= cache_cap)
                {
                    size_t new_cap = cache_cap ? cache_cap << 1 : 64;
                    seq_t *tmp = (seq_t *)realloc(cache,
                                                  new_cap * sizeof(seq_t));
                    if (!tmp)
                        abort(); /* allocation failure */
                    cache     = tmp;
                    cache_cap = new_cap;
                }

                cache[computed] = (seq_t)candidate; /* store k            */
                ++computed;
                ++candidate;      /* next candidate after this one */
                break;            /* proceed to find next term      */
            }
        }
    }

    return cache[(size_t)n - 1];
}
