/*
 * A142641  Primes congruent to 1 mod 56.
 * https://oeis.org/A142641
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   The n-th prime congruent to 1 (mod 56)
 * Range:     u8 n <= 1, u16 n <= 256, u32 n <= 155, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   grow_seq, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime_big.h"
#include "../utils/grow_seq.h"

seq_t a(idx_t n)
{
    static seq_t *primes = NULL;
    static size_t primes_sz = 0;
    static size_t primes_cap = 0;
    static seq_t next_candidate = 1; /* start searching from 1 */

    if (n == 0) /* undefined index, return 0 */
        return 0;

    while (primes_sz < n)
    {
        if (is_prime_big(next_candidate))
        {
            if (primes_sz == primes_cap)
                /* Ensure enough space for the next element */
                grow_seq(&primes, &primes_cap, primes_sz + 1);

            primes[primes_sz++] = next_candidate;
        }
        next_candidate += 56; /* Only numbers ≡ 1 (mod 56) need to be tested */
    }

    return primes[(size_t)(n - 1)];
}
