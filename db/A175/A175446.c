/*
 * A175446  a(n)>a(n-1), a(n) = smallest prime such that a(n)+a(n-1) is
 *          multiple of m, a(1)=2, m=11.
 * https://oeis.org/A175446
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 2; for n>1, a(n) = the smallest prime > a(n-1) * such
 *            that (a(n) + a(n-1)) mod 11 == 0. * a(1)=2, a(2)=31, a(3)=79,
 *            a(4)=97, …
 * Range:     u8 n <= 10, u16 n <= 678, u32 n <= 679, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   grow_seq, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/grow_seq.h"

/* returns the nth term of the sequence (n >= 1) */
seq_t a(idx_t n)
{
    if (n == 0) /* offset is 1, so n==0 is undefined */
        return 0;

    /* Cache for previously computed terms.  cache[i] holds a(i+1). */
    static seq_t *cache     = NULL;
    static idx_t cache_size = 0;  /* number of stored terms */
    static size_t cache_cap = 0;  /* allocated capacity of cache */

    /* Initialise the cache with a(1) if it is empty. */
    if (cache_size == 0) {
        grow_seq(&cache, &cache_cap, 1);
        cache[0] = (seq_t)2;   /* a(1) */
        cache_size = 1;
    }

    /* Grow the cache up to n if necessary. */
    if (n > cache_size) {
        grow_seq(&cache, &cache_cap, (size_t)n);

        for (idx_t i = cache_size; i < n; ++i) {        /* compute a(i+1) */
            seq_t prev = cache[i - 1];                 /* a(i)      */
            /* Residue needed so that (prev + candidate) % 11 == 0 */
            seq_t target_mod = (11 - (prev % 11)) % 11;

            /* First candidate > prev with this residue. */
            seq_t cand = prev + (seq_t)1;
            seq_t cand_mod = cand % 11;
            seq_t delta;
            if (cand_mod <= target_mod)
                delta = target_mod - cand_mod;
            else
                delta = target_mod + 11 - cand_mod;
            cand += delta;
            if (cand <= prev)                           /* safety */
                cand += 11;

            /* Search for the first prime with the required property. */
            while (!is_prime_big(cand))
                cand += 11;

            cache[i] = cand;
        }

        cache_size = n;
    }

    return cache[n - 1];
}
