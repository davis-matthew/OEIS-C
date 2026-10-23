/*
 * A141858  Primes congruent to 2 mod 13.
 * https://oeis.org/A141858
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 539, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   ensure_capacity, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   OEIS A141858: primes congruent to 2 (mod 13)  */
/*   a(n)  =  the nth prime p such that p % 13 == 2   */
#include <stddef.h>
#include "../utils/is_prime.h"
#include "../utils/ensure_capacity.h"

static seq_t *cache = NULL;       /* cached terms */
static size_t cache_size = 0;     /* number of cached terms */
static size_t cache_cap   = 0;     /* allocated capacity of cache */

seq_t a(idx_t n)
{
    /* The function works for any value of n >= 1.  Calls may arrive
       in arbitrary order; the cache grows lazily only when needed.  */

    if (n == 0) {          /* safety guard – the sequence is 1‑based */
        return 0;
    }

    /* Grow the cache until it contains at least n terms */
    if (cache_size < (size_t)n) {
        idx_t k;                      /* index of the current multiple of 13 */
        if (cache_size == 0) {
            k = 0;                    /* candidate = 13*0+2 = 2 */
        } else {
            /* Compute k for the first candidate after the last cached prime */
            k = ((idx_t)cache[cache_size - 1] - 2) / 13 + 1;
        }

        while (cache_size < (size_t)n) {
            idx_t candidate = 13 * k + 2;
            if (is_prime(candidate)) {                    /* test primality */
                ensure_capacity(cache_size + 1, &cache_cap,
                                (void **)&cache, sizeof(seq_t));
                cache[cache_size++] = (seq_t)candidate;   /* store in cache */
            }
            ++k;   /* next candidate 13*j+2 */
        }
    }

    return cache[n - 1];
}
