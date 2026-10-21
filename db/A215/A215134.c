/*
 * A215134  Primes congruent to {1, 2, 3} mod 11.
 * https://oeis.org/A215134
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   grow_seq, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A215134 – primes congruent to 1, 2 or 3 modulo 11. */
/* The function slowly builds a cache of qualifying primes; each call
   returns the n‑th such prime.  It uses the provided is_prime_big()
   routine from the utils directory. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"
#include "../utils/grow_seq.h"

seq_t a(idx_t n)
{
    /* The sequence is indexed from 1.  Behaviour for n==0 is
       undefined, so return 0 for completeness. */
    if (n == 0)
        return 0;

    /* Static cache storage */
    static seq_t *cache       = NULL;
    static size_t cache_cnt   = 0;   /* number of primes stored */
    static size_t cache_cap   = 0;   /* capacity of the cache array */
    static uint64_t last_checked = 1;/* last integer examined */

    /* Ensure the cache contains at least n primes */
    while (cache_cnt < n)
    {
        ++last_checked;

        /* Check the congruence condition */
        uint64_t r = last_checked % 11;
        if (r == 1 || r == 2 || r == 3)
        {
            if (is_prime_big((seq_t)last_checked))
            {
                /* Grow the cache if necessary */
                if (cache_cnt >= cache_cap)
                    grow_seq(&cache, &cache_cap, cache_cnt + 1);

                cache[cache_cnt++] = (seq_t)last_checked;
            }
        }
    }

    return cache[n - 1];
}
