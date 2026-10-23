/*
 * A123635  Residue mod 3 of average of n-th and (n+1)st odd primes.
 * https://oeis.org/A123635
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ((odd_prime(n) + odd_prime(n+1))/2) mod 3 where
 *            odd_prime(k) is the k-th odd prime (3,5,7,11,...)
 * Range:     u8 n <= 29, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/sieve_first_n_primes.h"

static uint32_t *prime_cache = NULL;
static size_t prime_cache_size = 0;

/* Ensure that prime_cache holds at least n+2 primes (including 2 at index 0). */
static void ensure_primes(idx_t n)
{
    size_t required = (size_t)n + 2;          /* indices 0 .. n+1 */
    if (prime_cache_size < required) {
        free(prime_cache);
        sieve_first_n_primes(required, &prime_cache, &prime_cache_size);
    }
}

seq_t a(idx_t n)
{
    if (n < 1)          /* offset starts at 1; behaviour below is irrelevant */
        return 0;

    ensure_primes(n);

    uint32_t p_n   = prime_cache[(size_t)n];       /* nth odd prime */
    uint32_t p_np1 = prime_cache[(size_t)n + 1];   /* (n+1)th odd prime */

    seq_t sum = (seq_t)p_n + (seq_t)p_np1;   /* always even */
    seq_t avg = sum / 2;                     /* exact integer division */
    return avg % 3;                          /* residue modulo 3 */
}
