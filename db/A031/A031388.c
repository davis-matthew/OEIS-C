/*
 * A031388  a(n) = prime(7*n-3).
 * https://oeis.org/A031388
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(7*n-3)
 * Range:     u8 n <= 8, u16 n <= 935, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/sieve_first_n_primes.h"

seq_t a(idx_t n)
{
    /* The index of the desired prime */
    idx_t k = (idx_t)7 * n - (idx_t)3;
    /* k is guaranteed to be >= 1 for n >= 1 */
    static uint32_t *primes = NULL;
    static idx_t prime_limit = 0;
    static size_t prime_cnt = 0;

    if (k > prime_limit) {
        /* Compute more primes up to the needed index */
        sieve_first_n_primes(k, &primes, &prime_cnt);
        prime_limit = k;
    }

    /* primes array is 0‑indexed; return the k‑th prime */
    return (seq_t)primes[(size_t)(k - (idx_t)1)];
}
