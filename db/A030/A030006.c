/*
 * A030006  a(n) = (prime(n)-1)*(prime(n)-5)/12.
 * https://oeis.org/A030006
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (prime(n)-1)*(prime(n)-5)/12
 * Range:     u8 n <= 8, u16 n <= 61, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/sieve_first_n_primes.h"

static uint32_t *prime_cache = NULL;
static size_t prime_cache_count = 0;

static void ensure_prime_cache(idx_t n)
{
    if (prime_cache_count >= (size_t)n) return;

    size_t needed = (size_t)n;
    uint32_t *primes_tmp = NULL;
    size_t cnt_tmp = 0;

    /* obtain the first 'needed' primes */
    sieve_first_n_primes(needed, &primes_tmp, &cnt_tmp);

    /* replace old cache */
    free(prime_cache);
    prime_cache = primes_tmp;
    prime_cache_count = cnt_tmp;
}

seq_t a(idx_t n)
{
    if (n < 3) return (seq_t)0;

    ensure_prime_cache(n);

    /* prime(n) is the (n-1)-th element of the 0‑based array */
    uint32_t p32 = prime_cache[n - 1];
    seq_t p = (seq_t)p32;

    seq_t result = (p - 1);
    result *= (p - 5);
    result /= 12;

    return result;
}
