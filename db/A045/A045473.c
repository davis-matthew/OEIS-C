/*
 * A045473  Primes congruent to 6 mod 7.
 * https://oeis.org/A045473
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 860, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence: primes congruent to 6 modulo 7.
 * Returns the n-th such prime (a(1) = 13, a(2) = 41, …).
 */
#include <stdlib.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    static seq_t *primes = NULL;
    static size_t primes_len = 0;
    static size_t primes_cap = 0;
    static seq_t candidate = 13;          /* first prime of the progression 7k+6 */

    if (n == 0)
        return 0;                      /* offset starts at 1 */

    while (primes_len < n)
    {
        if (is_prime_big(candidate))
        {
            if (primes_len == primes_cap)
            {
                size_t new_cap = primes_cap ? primes_cap << 1 : 16;
                seq_t *tmp = (seq_t *)realloc(primes, new_cap * sizeof(seq_t));
                primes = tmp;
                primes_cap = new_cap;
            }
            primes[primes_len++] = candidate;
        }
        candidate = candidate + 7;      /* next number of the form 7k+6 */
    }
    return primes[n - 1];
}
