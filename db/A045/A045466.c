/*
 * A045466  Primes congruent to {1, 4, 6} mod 7.
 * https://oeis.org/A045466
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime that is congruent to 1, 4, or 6 modulo 7
 * Range:     u8 n <= 25, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This implementation keeps a static cache of all primes found with the desired
 *  congruence.  On each call it extends the cache until the requested index
 *  is reached.  The check for primality uses the provided is_prime(idx_t)
 *  routine, and memory is reallocated using seq_realloc which works for all
 *  supported seq_t types.
 */

#include "../utils/is_prime.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /*  Static cache of discovered primes that satisfy the congruence.  */
    static seq_t *cache = NULL;    /* array of primes */
    static size_t cache_cap = 0;    /* allocated capacity */
    static size_t cache_cnt = 0;    /* number of primes stored */
    static idx_t last_checked = 1; /* last integer examined */

    /* Idx_t can be 0; behaviour for n < 1 is arbitrary, so just return 0. */
    if (n == (idx_t)0)
        return (seq_t)0;

    /* Extend cache until we have at least n primes. */
    while (cache_cnt < (size_t)n) {
        ++last_checked;

        /* skip numbers that are not 1, 4 or 6 modulo 7 */
        idx_t r = last_checked % 7;
        if (r != 1 && r != 4 && r != 6)
            continue;

        /* If prime, add to cache */
        if (is_prime(last_checked)) {
            if (cache_cnt == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                if (!tmp) return (seq_t)0; /* allocation failure – return 0 */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_cnt++] = (seq_t)last_checked;
        }
    }

    return cache[n - 1];
}
