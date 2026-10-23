/*
 * A105555  Let d = number of divisors of n; a(n) = d-th prime.
 * https://oeis.org/A105555
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = prime( d(n) ), where d(n) is the number of divisors of n
 * Range:     u8 n <= 5039, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdalign.h>
#include <stdlib.h>
#include <stdbool.h>

/* static data used only in this file */
static bool primes_initialized = false;
static seq_t *prime_list = NULL;
static size_t prime_count = 0;
static const size_t PRIME_LIMIT = 2000000; /* enough for any n in the tests */

/* initialise prime_list with all primes up to PRIME_LIMIT */
static void init_primes(void) {
    if (primes_initialized) return;

    size_t limit = PRIME_LIMIT;
    bool *is_prime = (bool *)malloc((limit + 1) * sizeof(bool));
    if (!is_prime) exit(1);
    for (size_t i = 0; i <= limit; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (size_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= limit; j += i) is_prime[j] = false;
        }
    }

    /* count primes and store them */
    prime_count = 0;
    for (size_t i = 2; i <= limit; ++i) if (is_prime[i]) ++prime_count;

    prime_list = (seq_t *)malloc(prime_count * sizeof(seq_t));
    if (!prime_list) exit(1);
    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (is_prime[i]) prime_list[idx++] = (seq_t)i;

    free(is_prime);
    primes_initialized = true;
}

/* compute a(n) = the prime at position d(n) */
seq_t a(idx_t n) {
    if (!primes_initialized) init_primes();
    if (n == 0) return 0;                     /* undefined, but safe */

    idx_t m = n;
    seq_t d = 1;

    /* factor m using the pre‑computed primes */
    for (size_t i = 0; i < prime_count &&
         ( (idx_t)prime_list[i] * (idx_t)prime_list[i] <= m ); ++i) {

        idx_t p = (idx_t)prime_list[i];
        if (m % p == 0) {
            seq_t exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            d *= (exp + 1);
        }
    }
    if (m > 1) d *= 2;          /* remaining prime factor */

    size_t pos = (size_t)d - 1; /* zero‑based index */
    return prime_list[pos];
}
