/*
 * A106482  Number of prime factors (with multiplicity) of A007588(n).
 * https://oeis.org/A106482
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A001222(A007588(n))
 * Range:     u8 n <= 6, u16 n <= 32, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = number of prime factors (with multiplicity) of n*(2*n^2-1) */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint32_t *prime_list = NULL;
static size_t prime_count  = 0;
static size_t prime_cap    = 0;
static bool    primes_ready = false;

/* Append a new prime, expanding the array if necessary */
static void add_prime(uint32_t p) {
    if (prime_count == prime_cap) {
        prime_cap = prime_cap ? prime_cap * 2 : 2048;
        uint32_t *tmp = (uint32_t *)realloc(prime_list, prime_cap * sizeof(uint32_t));
        if (!tmp) exit(EXIT_FAILURE);
        prime_list = tmp;
    }
    prime_list[prime_count++] = p;
}

/* Simple sieve up to a bound sufficient for n up to ~10 000 */
static void init_primes(void) {
    if (primes_ready) return;
    const uint32_t LIMIT = 2000000u;
    bool *is_comp = (bool *)calloc(LIMIT + 1, sizeof(bool));
    if (!is_comp) exit(EXIT_FAILURE);
    for (uint32_t i = 2; i <= LIMIT; ++i) {
        if (!is_comp[i]) {
            add_prime(i);
            if ((uint64_t)i * i <= LIMIT) {
                for (uint32_t j = i * i; j <= LIMIT; j += i)
                    is_comp[j] = true;
            }
        }
    }
    free(is_comp);
    primes_ready = true;
}

/* Count prime factors (with multiplicity) of n*(2*n^2-1) */
seq_t a(idx_t n) {
    if (n <= 1) return 0;         /* a(0) = 0, a(1) = 0 by definition */

    init_primes();

    /* Compute m = n * (2*n^2 - 1) in the generic integer type seq_t */
    seq_t sn = (seq_t)n;
    seq_t m  = sn * ((seq_t)2 * sn * sn - (seq_t)1);

    seq_t count = 0;
    seq_t tmp = m;

    for (size_t i = 0; i < prime_count && (seq_t)prime_list[i] * (seq_t)prime_list[i] <= tmp; ++i) {
        uint32_t p = prime_list[i];
        while (tmp % (seq_t)p == (seq_t)0) {
            tmp /= (seq_t)p;
            count += (seq_t)1;
        }
    }
    /* If a prime factor > sqrt(m) remains */
    if (tmp > (seq_t)1) {
        count += (seq_t)1;
    }
    return count;
}
