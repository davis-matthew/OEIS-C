/*
 * A116015  n+sigma(n)+sigma(sigma(n)) is a triangular number.
 * https://oeis.org/A116015
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth integer m such that m + σ(m) + σ(σ(m)) is a
 *            triangular number. (σ is the sum‑of‑divisors function)
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

/* ----------------- prime sieve (dynamic) ----------------- */
static uint32_t *prime_array = NULL;   /* list of primes */
static size_t   prime_cap   = 0;        /* capacity of prime_array   */
static size_t   prime_cnt   = 0;        /* number of primes stored   */

static bool *is_prime = NULL;          /* sieve array (true = prime) */
static uint32_t sieve_max = 0;          /* highest index sieved so far */

/* ensure primes up to 'limit' are generated and stored */
static void extend_sieve(uint32_t limit)
{
    if (limit <= sieve_max) return;

    /* grow the sieve array */
    bool *new_is_prime = (bool *)realloc(is_prime, (limit + 1) * sizeof(bool));
    if (!new_is_prime) exit(1);
    is_prime = new_is_prime;

    /* initialise the new entries as prime */
    for (uint32_t i = sieve_max + 1; i <= limit; ++i) is_prime[i] = true;
    if (sieve_max == 0) { is_prime[0] = is_prime[1] = false; }

    /* cross off composites using known primes up to sqrt(limit) */
    uint32_t max_sqrt = (uint32_t)floor_sqrt((seq_t)limit);
    for (size_t i = 0; i < prime_cnt && prime_array[i] <= max_sqrt; ++i) {
        uint32_t p  = prime_array[i];
        uint32_t start = (sieve_max + p) / p * p;
        if (start < p * p) start = p * p;
        for (uint32_t m = start; m <= limit; m += p) is_prime[m] = false;
    }

    /* add new primes to the list */
    for (uint32_t i = sieve_max + 1; i <= limit; ++i) {
        if (is_prime[i]) {
            if (prime_cnt == prime_cap) {
                prime_cap = prime_cap ? prime_cap * 2 : 8;
                uint32_t *tmp = (uint32_t *)realloc(prime_array, prime_cap * sizeof(uint32_t));
                if (!tmp) exit(1);
                prime_array = tmp;
            }
            prime_array[prime_cnt++] = i;
        }
    }

    sieve_max = limit;
}

/* ----------------- sigma function for 64-bit integers ----------------- */
static seq_t sigma_of_u64(uint64_t m)
{
    if (m == 0) return 0;
    /* ensure the prime list covers sqrt(m) */
    uint32_t limit = (uint32_t)floor_sqrt((seq_t)m);
    extend_sieve(limit);

    seq_t result = 1;
    uint64_t n = m;

    for (size_t i = 0; i < prime_cnt && (uint64_t)prime_array[i] * (uint64_t)prime_array[i] <= n; ++i) {
        uint32_t p = prime_array[i];
        if (n % p == 0) {
            seq_t term = 1;
            seq_t pow  = 1;
            while (n % p == 0) {
                n /= p;
                pow *= (seq_t)p;
                term += pow;
            }
            result *= term;
        }
    }

    if (n > 1) { /* remaining prime factor */
        seq_t term = 1 + (seq_t)n;
        result *= term;
    }

    return result;
}

/* ----------------- sequence A116015 ----------------- */
static seq_t *term_array = NULL;   /* cached terms of A116015 */
static size_t term_len   = 0;      /* number of terms stored     */
static size_t term_cap   = 0;      /* capacity of term_array     */
static uint64_t last_checked = 1;  /* candidate number to test   */

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset 1; n >= 1 by specification */

    /* initialise primes up to 2 */
    if (prime_cnt == 0) extend_sieve(2);

    /* grow term cache until we have at least n terms */
    while (term_len < (size_t)n) {
        uint64_t m = last_checked;

        seq_t sigma_m      = sigma_of_u64(m);
        uint64_t sigma_small = (uint64_t)sigma_m;          /* safe: sigma(m) is small for our range */
        seq_t sigma_sigma  = sigma_of_u64(sigma_small);

        seq_t sum = (seq_t)m + sigma_m + sigma_sigma;
        seq_t D   = 1 + 8 * sum;
        seq_t sqrtD = floor_sqrt(D);

        if (sqrtD * sqrtD == D && ((sqrtD - 1) & 1) == 0) { /* triangular test */
            if (term_len == term_cap) {
                term_cap = term_cap ? term_cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(term_array, term_cap * sizeof(seq_t));
                if (!tmp) exit(1);
                term_array = tmp;
            }
            term_array[term_len++] = (seq_t)m;
        }

        ++last_checked;
    }

    return term_array[n - 1];
}
