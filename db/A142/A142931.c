/*
 * A142931  Primes congruent to 13 mod 64.
 * https://oeis.org/A142931
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 204, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   grow_seq, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: primes that are ≡13 (mod 64) */

#include <stdint.h>
#include <stddef.h>
#include "../utils/is_prime.h"
#include "../utils/grow_seq.h"

seq_t a(idx_t n)
{
    /* cache of computed primes */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static idx_t next_k = 0;

    if (n == 0) {
        /* Undefined for 0, return 0 as a placeholder */
        return 0;
    }

    /* return cached value if already computed */
    if (n <= cache_len) {
        return cache[n - 1];
    }

    /* ensure cache can hold n elements */
    grow_seq(&cache, &cache_cap, n);

    /* find additional primes of the form 64*k+13 */
    while (cache_len < (size_t)n) {
        idx_t candidate = (idx_t)64ULL * next_k + 13;  /* candidate prime */
        if (is_prime(candidate)) {
            cache[cache_len++] = (seq_t)candidate;
        }
        next_k++;
    }

    return cache[n - 1];
}
