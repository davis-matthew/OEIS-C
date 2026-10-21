/*
 * A071411  "Sum of n first primes" minus "sum of first n nonprimes".
 * https://oeis.org/A071411
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = (sum of first n primes) – (sum of first n non‑primes)
 * Range:     i8 n <= 14, i16 n <= 59, i32 n <= 59, i64 n <= 59, i128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This implementation uses a simple sieve of Eratosthenes to generate
    the primes up to a current limit.  The list of primes and the list of
    non‑primes (1 and the composites) are stored together with their
    prefix sums.  When n exceeds the pre‑computed number of primes or
    non‑primes the tables are rebuilt with a larger limit.  */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static const size_t INITIAL_LIMIT = 200000;   /* enough for n≈10000 */

/* Global state */
static bool init_done = false;
static size_t limit = INITIAL_LIMIT;

static uint64_t *primes = NULL;
static seq_t *sum_primes = NULL;
static size_t prime_cnt = 0;

static uint64_t *nonprimes = NULL;
static seq_t *sum_nonprimes = NULL;
static size_t nonprime_cnt = 0;

/* Forward declaration */
static void rebuild_tables(void);

/* Build prime list, non‑prime list and their prefix sums for the current
   limit.  All allocations are casted for C++ compatibility. */
static void rebuild_tables(void)
{
    /* Sieve of Eratosthenes */
    bool *is_prime = (bool*)calloc(limit + 1, sizeof(bool));
    if (!is_prime) exit(1);
    for (size_t i = 2; i <= limit; ++i) is_prime[i] = true;
    for (size_t i = 2; i * i <= limit; ++i)
        if (is_prime[i])
            for (size_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;

    /* Count primes */
    size_t pcnt = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++pcnt;

    /* Allocate prime array */
    primes = (uint64_t*)malloc(pcnt * sizeof(uint64_t));
    if (!primes) exit(1);
    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (is_prime[i]) primes[idx++] = i;

    /* Build prefix sum of primes */
    sum_primes = (seq_t*)calloc(pcnt + 1, sizeof(seq_t));
    if (!sum_primes) exit(1);
    sum_primes[0] = 0;
    for (size_t i = 0; i < pcnt; ++i)
        sum_primes[i + 1] = sum_primes[i] + (seq_t)primes[i];

    /* Count non‑primes (1 and composites) */
    size_t ncnt = 0;
    for (size_t i = 1; i <= limit; ++i)
        if (!(i >= 2 && is_prime[i])) ++ncnt;

    /* Allocate non‑prime array */
    nonprimes = (uint64_t*)malloc(ncnt * sizeof(uint64_t));
    if (!nonprimes) exit(1);
    idx = 0;
    for (size_t i = 1; i <= limit; ++i)
        if (!(i >= 2 && is_prime[i])) nonprimes[idx++] = i;

    /* Build prefix sum of non‑primes */
    sum_nonprimes = (seq_t*)calloc(ncnt + 1, sizeof(seq_t));
    if (!sum_nonprimes) exit(1);
    sum_nonprimes[0] = 0;
    for (size_t i = 0; i < ncnt; ++i)
        sum_nonprimes[i + 1] = sum_nonprimes[i] + (seq_t)nonprimes[i];

    /* Clean up */
    free(is_prime);

    /* Store counts for future checks */
    prime_cnt = pcnt;
    nonprime_cnt = ncnt;
}

/* Ensure tables contain at least n primes and n non‑primes. */
static void init_arrays(void)
{
    if (init_done) return;
    rebuild_tables();
    init_done = true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    init_arrays();

    /* If the current tables are not big enough, grow them. */
    if ((size_t)n > prime_cnt || (size_t)n > nonprime_cnt) {
        /* Double the limit until it covers n for both lists. */
        while ((size_t)n > prime_cnt || (size_t)n > nonprime_cnt) {
            limit *= 2;
        }
        /* Free old tables. */
        free(primes);
        free(sum_primes);
        free(nonprimes);
        free(sum_nonprimes);
        init_done = false;
        rebuild_tables();
        init_done = true;
    }

    /* Return the difference of prefix sums.  Cast n to size_t for indexing. */
    return sum_primes[(size_t)n] - sum_nonprimes[(size_t)n];
}
