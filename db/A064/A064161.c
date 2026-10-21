/*
 * A064161  Least abundant number divisible by the n-th prime number.
 * https://oeis.org/A064161
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 6 * prime(n) for n >= 4
 *             a(1) = 12, a(2) = 12, a(3) = 20
 *
 * The function returns the least abundant number divisible by the n-th
 * prime.  For n >= 4 the result is simply six times the n-th prime.
 * The first three terms are handled explicitly because the closed form
 * does not apply there.
 */

#include <stdlib.h>   /* malloc, realloc, free */
#include <stdint.h>   /* uint32_t, uint8_t */
#include <stdbool.h>
#include <math.h>     /* sqrt */

static uint32_t *prime_list   = NULL;   /* array of primes */
static size_t    prime_count  = 0;      /* number of primes stored */

/* Build a prime table up to LIMIT.  LIMIT must be large enough to
 * contain the requested number of primes (e.g. for n <= 10000,
 * LIMIT = 200000 yields 17984 primes). */
static void init_prime_table(void)
{
    if (prime_list != NULL) return;   /* already initialized */

    const uint32_t LIMIT = 200000;      /* sufficient for n <= 10000 */
    uint8_t *is_prime = (uint8_t *)malloc(LIMIT + 1);
    if (!is_prime) return;             /* allocation failed; caller will handle */

    /* Simple sieve of Eratosthenes */
    for (uint32_t i = 0; i <= LIMIT; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;
    for (uint32_t i = 2; i * i <= LIMIT; ++i) {
        if (is_prime[i]) {
            for (uint32_t j = i * i; j <= LIMIT; j += i) {
                is_prime[j] = 0;
            }
        }
    }

    /* Dynamic array to collect primes */
    size_t capacity = 128;
    prime_list = (uint32_t *)malloc(capacity * sizeof(uint32_t));
    if (!prime_list) {
        free(is_prime);
        return;
    }

    for (uint32_t i = 2; i <= LIMIT; ++i) {
        if (is_prime[i]) {
            if (prime_count == capacity) {
                capacity *= 2;
                uint32_t *tmp = (uint32_t *)realloc(prime_list, capacity * sizeof(uint32_t));
                if (!tmp) break; /* allocation failed; stop adding primes */
                prime_list = tmp;
            }
            prime_list[prime_count++] = i;
        }
    }

    free(is_prime);
}

/* Return the n-th prime (1‑based).  Assumes n is at least 1. */
static uint32_t get_prime(idx_t n)
{
    init_prime_table();
    if (n == 0 || (size_t)n > prime_count) return 0;
    return prime_list[n - 1];
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)12;
    if (n == 2) return (seq_t)12;
    if (n == 3) return (seq_t)20;

    uint32_t p = get_prime(n);
    return (seq_t)6 * (seq_t)p;
}
