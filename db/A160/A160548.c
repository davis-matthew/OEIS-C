/*
 * A160548  Primes of the form k^2 + k + 844427.
 * https://oeis.org/A160548
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th prime of the form k^2 + k + 844427, with k
 *            starting at 0
 * Range:     u8 none, u16 none, u32 none, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   ensure_capacity, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/is_prime_big.h"
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n)
{
    /* static cache for already discovered primes */
    static seq_t *cache = NULL;          /* array of primes found */
    static size_t cache_cap = 0;         /* allocated capacity */
    static size_t cache_size = 0;        /* number of primes stored */
    static idx_t next_k = 0;             /* next k to evaluate */

    /* extend cache until it contains at least n primes */
    while (cache_size < n) {
        seq_t k   = (seq_t)next_k;
        seq_t val = k * (k + (seq_t)1) + (seq_t)844427;

        if (is_prime_big(val)) {
            /* enlarge the cache if needed */
            if (cache_size + 1 > cache_cap)
                ensure_capacity(cache_size + 1, &cache_cap,
                                (void **)&cache, sizeof(seq_t));
            cache[cache_size++] = val;
        }
        ++next_k;
    }

    return cache[n - 1];
}
