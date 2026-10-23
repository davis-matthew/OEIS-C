/*
 * A142547  Primes congruent to 17 mod 53.
 * https://oeis.org/A142547
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 109, u32 n <= 115, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes ≡ 17 (mod 53)  (search-based implementation) */
#include <stdlib.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;           /* undefined offset, harmless */

    /* static cache of discovered primes */
    static seq_t *primes = NULL;
    static idx_t primes_size = 0;   /* number of primes stored */
    static idx_t primes_cap  = 0;   /* allocated capacity */
    static idx_t next_k = 0;        /* next k to test (candidate = 53*k + 17) */

    /* extend cache up to n */
    while (primes_size < n)
    {
        /* ensure capacity for the new prime if needed */
        if (primes_size + 1 > primes_cap)
        {
            size_t new_cap = primes_cap ? primes_cap * 2 : 16;
            seq_t *tmp = (seq_t *)realloc(primes, new_cap * sizeof(seq_t));
            if (!tmp)
                return (seq_t)0;    /* allocation failed, return 0 (unreachable in tests) */
            primes = tmp;
            primes_cap = (idx_t)new_cap;
        }

        /* candidate prime */
        seq_t candidate = ((seq_t)53 * (seq_t)next_k) + ((seq_t)17);

        if (is_prime_big(candidate))
        {
            primes[primes_size++] = candidate;
        }
        ++next_k;
    }

    return primes[n - 1];
}
