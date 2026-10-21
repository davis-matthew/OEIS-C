/*
 * A096554  Output of the linear congruential pseudo-random number generator
 *          used in function rand() as described in Kernighan and Ritchie,
 *          when seeded with 0.
 * https://oeis.org/A096554
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(x(n)/2^16) where x(0) = 0 x(n) = (1103515245 *
 *            x(n-1) + 12345) mod 2^31 This is the sequence of the 31‑bit
 *            linear congruential generator used in the standard C rand(),
 *            seeded with 0. The function implements this recurrence
 *            iteratively and memoises terms so that repeated calls are O(1...
 * Range:     u8 n <= 1, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* static storage for memoised values =
   cache[i] holds a(i+1).  cache_len records how many terms are in
   the cache (i.e. the largest n that has been computed).  last_x is
   the internal 31‑bit state x(cache_len).  All arithmetic for the
   generator is performed in 64‑bit unsigned integers to avoid
   overflow.                                           */
static seq_t *cache      = NULL;
static idx_t  cache_len  = 0;          /* number of terms available */
static unsigned long long last_x = 0ULL;/* x(cache_len), 0 <= x < 2^31 */

/* public routine ---------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) /* offset 1 should not be queried, but return 0 for safety */
        return (seq_t)0;

    /* Expand the cache to contain all terms up to n if necessary.      */
    if (cache_len < n)
    {
        size_t old_len = (size_t)cache_len;
        size_t new_len = (size_t)n;

        seq_t *new_cache = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache)
            abort(); /* memory allocation failed */
        cache = new_cache;

        unsigned long long x = last_x;
        for (idx_t i = old_len; i < n; ++i) {
            x = (1103515245ULL * x + 12345ULL) & 0x7FFFFFFFULL; /* 2^31-1 mask */
            cache[i] = (seq_t)(x >> 16);                         /* floor(x/2^16) */
        }
        last_x     = x;
        cache_len  = n;
    }

    return cache[n-1]; /* cache[0] holds a(1) */
}
