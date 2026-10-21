/*
 * A221903  Primes of the form 2*n^2 + 42*n + 19.
 * https://oeis.org/A221903
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n^2 + 42*n + 19 (prime)
 * Range:     u8 n <= 1, u16 n <= 17, u32 n <= 20, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   grow_seq_array, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

#include "../utils/is_prime_big.h"
#include "../utils/grow_seq_array.h"

seq_t a(idx_t n)
{
    /* Cache of the primes that have already been found */
    static seq_t *primes = NULL;
    static size_t primes_cap = 0;
    static size_t primes_len = 0;
    static idx_t next_n = 1;      /* next integer n to test */

    /* Generate primes until we have at least n of them */
    while (primes_len < (size_t)n)
    {
        seq_t val = 2 * ((seq_t)next_n * (seq_t)next_n) + 42 * (seq_t)next_n + 19;

        if (is_prime_big(val))
        {
            /* Ensure the array can hold another element */
            grow_seq_array(&primes, &primes_cap, primes_len + 1);
            primes[primes_len++] = val;
        }
        ++next_n;
    }

    return primes[n - 1];
}
