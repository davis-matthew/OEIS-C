/*
 * A175510  (2*n)-th twin prime minus 2*(n-th twin prime).
 * https://oeis.org/A175510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A001097(2n) - 2*A001097(n) where A001097 lists primes
 *            that are part of a twin‑prime pair (i.e. primes p with p-2 or
 *            p+2 also prime). This function lazily builds a sieve up to a
 *            fixed limit and stores that sequence in a dynamic array. It then
 *            computes a(n)
 * Range:     i8 n <= 20, i16 n <= 52, i32 n <= 52, i64 n <= 52, i128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* static state --------------------------------------------------- */
static bool *prime_sieve = NULL;          /* sieve of Eratosthenes */
static size_t sieve_limit = 0;            /* used limit of sieve */

static seq_t *twin_primes = NULL;         /* primes in twin pairs */
static size_t twin_capacity = 0;          /* allocated capacity */
static size_t twin_count   = 0;           /* number of primes stored */

/* helper ---------------------------------------------------------- */
static void build_twin_primes(void)
{
    /* choose a limit that comfortably contains at least 20000 such
       primes (two per twin pair). 5 000 000 is far more than enough
       for n ≤ 10 000. */
    const size_t LIMIT = 5000000UL;
    sieve_limit = LIMIT;

    prime_sieve = (bool *)calloc(sieve_limit + 3, 1);  /* +3 for p+2 */
    if (!prime_sieve) abort();

    /* initialize all as prime */
    for (size_t i = 2; i <= sieve_limit + 2; ++i) prime_sieve[i] = true;

    /* Eratosthenes */
    for (size_t p = 2; p * p <= sieve_limit + 2; ++p) {
        if (prime_sieve[p]) {
            for (size_t j = p * p; j <= sieve_limit + 2; j += p)
                prime_sieve[j] = false;
        }
    }

    /* allocate initial capacity for twin primes */
    twin_capacity = 10000U;          /* rough initial estimate */
    twin_primes   = (seq_t *)malloc(twin_capacity * sizeof(seq_t));
    if (!twin_primes) abort();

    twin_count = 0;
    for (size_t p = 3; p <= sieve_limit; p += 2) { /* only odd primes */
        if (!prime_sieve[p]) continue;
        if ((p >= 3 && prime_sieve[p - 2]) || prime_sieve[p + 2]) {
            if (twin_count == twin_capacity) {
                twin_capacity *= 2;
                seq_t *tmp = (seq_t *)realloc(twin_primes,
                                              twin_capacity * sizeof(seq_t));
                if (!tmp) abort();
                twin_primes = tmp;
            }
            twin_primes[twin_count++] = (seq_t)p;
        }
    }

    /* sieve memory no longer needed */
    free(prime_sieve);
    prime_sieve = NULL;
}

/* ---------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* behaviour below offset not required */

    /* lazy initialization */
    if (!twin_primes) build_twin_primes();

    /* ensure we have at least 2n primes */
    const size_t m = (size_t)n * 2;
    if (m > twin_count) {
        /* unlikely for n ≤ 10000, but re‑build with larger limit if needed */
        free(twin_primes);
        twin_primes = NULL;
        /* double the limit until sufficient */
        size_t new_limit = sieve_limit * 2;
        for (;;) {
            /* recreate sieve with larger limit */
            sieve_limit = new_limit;
            prime_sieve = (bool *)calloc(sieve_limit + 3, 1);
            if (!prime_sieve) abort();
            for (size_t i = 2; i <= sieve_limit + 2; ++i) prime_sieve[i] = true;
            for (size_t p = 2; p * p <= sieve_limit + 2; ++p) {
                if (prime_sieve[p]) {
                    for (size_t j = p * p; j <= sieve_limit + 2; j += p)
                        prime_sieve[j] = false;
                }
            }
            twin_capacity = 10000U;
            twin_primes   = (seq_t *)malloc(twin_capacity * sizeof(seq_t));
            if (!twin_primes) abort();
            twin_count = 0;
            for (size_t p = 3; p <= sieve_limit; p += 2) {
                if (!prime_sieve[p]) continue;
                if ((p >= 3 && prime_sieve[p - 2]) || prime_sieve[p + 2]) {
                    if (twin_count == twin_capacity) {
                        twin_capacity *= 2;
                        seq_t *tmp = (seq_t *)realloc(twin_primes,
                                                      twin_capacity * sizeof(seq_t));
                        if (!tmp) abort();
                        twin_primes = tmp;
                    }
                    twin_primes[twin_count++] = (seq_t)p;
                }
            }
            free(prime_sieve);
            prime_sieve = NULL;
            if (m <= twin_count) break;
            new_limit *= 2;
        }
    }

    /* indices are 0‑based in array */
    seq_t first  = twin_primes[(size_t)n - 1];
    seq_t second = twin_primes[(size_t)2 * n - 1];
    return second - (seq_t)2 * first;
}
