/*
 * A139554  a(n) = lcm(1..floor(n/4)).
 * https://oeis.org/A139554
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm(1..floor(n/4))
 * Range:     u8 n <= 27, u16 n <= 51, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Static storage for primes up to the largest needed limit. */
static idx_t *prime_list = NULL;
static size_t prime_count = 0;
static idx_t prime_limit = 0;

/* Ensure that prime_list contains all primes ≤ limit. */
static void ensure_primes(idx_t limit) {
    if (limit <= prime_limit) return;

    /* Free existing list if we had to enlarge it. */
    if (prime_list) free(prime_list);

    /* Simple Eratosthenes sieve up to `limit`. */
    idx_t size = limit + 1;
    bool *is_prime = (bool*)malloc(size);
    for (idx_t i = 0; i <= limit; ++i) is_prime[i] = true;
    if (limit >= 0) is_prime[0] = false;
    if (limit >= 1) is_prime[1] = false;

    for (idx_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= limit; j += i) {
                is_prime[j] = false;
            }
        }
    }

    /* Count and store primes. */
    size_t count = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (is_prime[i]) ++count;
    }

    prime_list = (idx_t*)malloc(count * sizeof(idx_t));
    size_t pos = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (is_prime[i]) prime_list[pos++] = i;
    }

    prime_count = count;
    prime_limit = limit;
    free(is_prime);
}

/* Main sequence entry point. */
seq_t a(idx_t n)
{
    idx_t m = n / 4;
    if (m <= 1) return (seq_t)1;   /* lcm(1..0) = 1, lcm(1..1) = 1 */

    ensure_primes(m);

    seq_t result = (seq_t)1;
    for (size_t idx = 0; idx < prime_count; ++idx) {
        seq_t p = (seq_t)prime_list[idx];
        seq_t power = p;
        /* Highest power of p not exceeding m. */
        while (power <= (seq_t)m / p) {
            power *= p;
        }
        result *= power;
    }
    return result;
}
