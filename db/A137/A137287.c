/*
 * A137287  a(n) is the number 2 written (prime(n)-1)/2 times followed by the
 *          digit 1; a(1)=2.
 * https://oeis.org/A137287
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = 2 repeated (prime(n)-1)/2 times followed by 1; a(1) = 2
 *            The sequence uses the nth prime (prime(1)=2). Parameters idx_t
 *            and seq_t are supplied by the build
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 8, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint32_t *primes = NULL;          /* array of primes found */
static size_t prime_count = 0;           /* number of primes stored */
static const size_t PRIME_SIEVE_LIMIT = 200000; /* enough for n up to ~18000 */

/* Simple sieve of Eratosthenes to fill primes array up to LIMIT */
static void build_primes_upto(size_t limit)
{
    /* Allocate boolean array for primality flags */
    uint8_t *is_prime = (uint8_t *)malloc(limit + 1);
    if (!is_prime) return; /* allocation failure; caller must check */
    memset(is_prime, 1, limit + 1);
    if (limit >= 0) {
        is_prime[0] = 0;
        if (limit >= 1) is_prime[1] = 0;
    }

    for (size_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= limit; j += i) {
                is_prime[j] = 0;
            }
        }
    }

    /* Count primes to determine needed storage */
    for (size_t i = 2; i <= limit; ++i) {
        if (is_prime[i]) ++prime_count;
    }

    /* Allocate storage for primes */
    primes = (uint32_t *)realloc(primes, prime_count * sizeof(uint32_t));
    if (!primes) return; /* allocation failure */

    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i) {
        if (is_prime[i]) {
            primes[idx++] = (uint32_t)i;
        }
    }
    free(is_prime);
}

/* Ensure that primes array has at least n primes (prime(1..n)) */
static void ensure_primes(idx_t n)
{
    if (prime_count >= (size_t)n) return;
    /* Build up to the fixed limit; this should cover all n we use */
    build_primes_upto(PRIME_SIEVE_LIMIT);
}

seq_t a(idx_t n)
{
    if (n == 1) {
        return (seq_t)2;
    }

    /* make sure we have at least n primes */
    ensure_primes(n);

    /* get the nth prime */
    uint32_t p = primes[n - 1]; /* primes[0] is 2 = prime(1) */

    /* compute number of leading '2's: k = (p-1)/2 */
    idx_t k = ((idx_t)p - 1) / 2;

    seq_t val = 0;

    /* append k copies of digit 2 */
    for (idx_t i = 0; i < k; ++i) {
        val = val * (seq_t)10 + (seq_t)2;
    }

    /* final digit 1 */
    val = val * (seq_t)10 + (seq_t)1;

    return val;
}
