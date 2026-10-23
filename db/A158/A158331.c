/*
 * A158331  List of twin primes p1 and p2 with the sums of digits in p1 and p2
 *          are twin primes.
 * https://oeis.org/A158331
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   digit_sum, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: twin primes p1 and p2 such that digit_sum(p1) and digit_sum(p2) are twin primes. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "../utils/digit_sum.h"
#include "../utils/is_prime_big.h"

/* static state ------------------------------------------------------------ */
static uint64_t sieve_limit = 0;
static char *sieve = NULL;
static uint64_t *primes = NULL;
static size_t prime_count = 0;

static seq_t *terms = NULL;
static size_t terms_capacity = 0;
static size_t terms_count = 0;
static size_t prime_scan_index = 0;

static bool initialized = false;

/* allocate a new array with at least new_cap elements and copy old data */
static void grow_terms(size_t new_cap)
{
    terms = (seq_t*)realloc(terms, new_cap * sizeof(seq_t));
    terms_capacity = new_cap;
}

/* simple sieve of Eratosthenes that stores the list of primes */
static void build_sieve(uint64_t limit)
{
    char *new_sieve = (char*)malloc(limit + 1);
    if (!new_sieve) return;  /* allocation failed – caller will handle */

    memset(new_sieve, 1, limit + 1);
    new_sieve[0] = new_sieve[1] = 0;

    for (uint64_t p = 2; p * p <= limit; ++p) {
        if (new_sieve[p]) {
            for (uint64_t m = p * p; m <= limit; m += p)
                new_sieve[m] = 0;
        }
    }

    size_t cnt = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (new_sieve[i]) ++cnt;

    uint64_t *new_primes = (uint64_t*)malloc(cnt * sizeof(uint64_t));
    if (!new_primes) {
        free(new_sieve);
        return;
    }

    size_t idx = 0;
    for (uint64_t i = 2; i <= limit; ++i) {
        if (new_sieve[i])
            new_primes[idx++] = i;
    }

    free(sieve);
    free(primes);

    sieve = new_sieve;
    primes = new_primes;
    prime_count = cnt;
}

/* ensure static state is ready for use */
static void init_static(void)
{
    if (initialized) return;
    sieve_limit = 100000;            /* initial limit, will grow when needed   */
    build_sieve(sieve_limit);
    terms_capacity = 0;
    terms = NULL;
    terms_count = 0;
    prime_scan_index = 0;
    initialized = true;
}

/* public function ---------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;

    init_static();

    /* extend the list of terms until we have at least n terms     */
    while (terms_count < (size_t)n) {
        /* ensure we still have a prime pair to inspect */
        while (prime_scan_index + 1 >= prime_count) {
            /* enlarge the sieve */
            uint64_t new_limit = sieve_limit * 2;
            build_sieve(new_limit);
            sieve_limit = new_limit;
            /* keep scanning from current position */
            if (prime_scan_index >= prime_count - 1)
                prime_scan_index = prime_count - 2;
        }

        uint64_t p1 = primes[prime_scan_index];
        uint64_t p2 = primes[prime_scan_index + 1];
        if (p2 == p1 + 2ULL) {
            seq_t s1 = digit_sum((seq_t)p1);
            seq_t s2 = digit_sum((seq_t)p2);
            if (is_prime_big(s1) && is_prime_big(s2) && (s2 - s1 == 2)) {
                /* store p1 and p2 as two consecutive terms */
                if (terms_capacity < terms_count + 2) {
                    size_t new_cap = terms_capacity ? terms_capacity * 2 : 64;
                    grow_terms(new_cap);
                }
                terms[terms_count++] = (seq_t)p1;
                terms[terms_count++] = (seq_t)p2;
            }
        }
        ++prime_scan_index;
    }

    return terms[n - 1];
}
