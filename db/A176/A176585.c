/*
 * A176585  Primes of the form n^3+Smallest square, (Smallest square >= n^3).
 * https://oeis.org/A176585
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   append_candidate, floor_sqrt, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = nth prime of the form i^3 + ceil(sqrt(i^3))^2, where i runs over
   positive integers. The sequence starts with a(1)=2 for i=1, a(2)=17 for
   i=2, a(3)=269 for i=5, and so on. */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/floor_sqrt.h"
#include "../utils/is_prime_big.h"
#include "../utils/append_candidate.h"

static seq_t *prime_cache      = NULL;   /* array of found primes */
static size_t prime_cache_len = 0;       /* number of cached primes        */
static size_t prime_cache_cap = 0;       /* allocated capacity             */
static idx_t  last_i           = 0;       /* last i processed (cumulative)  */

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* defensive, not used          */

    /* Extend the cache until it contains at least n primes. */
    while ((idx_t)prime_cache_len < n) {

        /* Next integer i to test. */
        last_i++;
        seq_t i_seq = (seq_t)last_i;

        /* Compute i^3. */
        seq_t i_cubed = i_seq * i_seq * i_seq;

        /* Smallest integer >= sqrt(i^3). */
        seq_t t = floor_sqrt(i_cubed);
        if (t * t < i_cubed)                 /* ceil sqrt                */
            t++;

        /* Candidate prime = i^3 + t^2. */
        seq_t candidate = i_cubed + t * t;

        /* Test primality and cache if prime. */
        if (is_prime_big(candidate))
            append_candidate(&prime_cache, &prime_cache_len,
                             &prime_cache_cap, candidate);
    }

    /* Return the requested nth prime (1‑based index). */
    return prime_cache[n - 1];
}
