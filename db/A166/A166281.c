/*
 * A166281  Number of ordered factorizations of the nonprimes (A018252).
 * https://oeis.org/A166281
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A002033(A018252(n)-1) Ordered factorizations of the n‑th
 *            nonprime number
 * Range:     u8 n <= 148, u16 n <= 7564, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* static data used for lazy initialization */
static bool initialized = false;
static idx_t SIEVE_LIMIT = 200000;          /* plenty for n <= 1e4 */
static seq_t *fact_counts   = NULL;         /* number of ordered factorizations */
static idx_t *nonprimes    = NULL;          /* list of nonprime numbers */
static size_t nonprime_cnt = 0;             /* size of the nonprimes array */

/* Helper: perform a simple sieve of Eratosthenes up to SIEVE_LIMIT */
static void init(void)
{
    /* 1. sieve primes */
    char *is_prime = (char*)malloc((SIEVE_LIMIT + 1) * sizeof(char));
    if (!is_prime) exit(1);
    for (idx_t i = 0; i <= SIEVE_LIMIT; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;
    for (idx_t i = 2; i * i <= SIEVE_LIMIT; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= SIEVE_LIMIT; j += i)
                is_prime[j] = 0;
        }
    }

    /* 2. build list of nonprimes (1 and composites) */
    nonprimes = (idx_t*)malloc((SIEVE_LIMIT + 1) * sizeof(idx_t));
    if (!nonprimes) exit(1);
    size_t idx = 0;
    for (idx_t i = 1; i <= SIEVE_LIMIT; ++i) {
        if (i == 1 || !is_prime[i]) {
            nonprimes[idx++] = i;
        }
    }
    nonprime_cnt = idx;

    /* 3. compute number of ordered factorizations for all n <= SIEVE_LIMIT */
    fact_counts = (seq_t*)malloc((SIEVE_LIMIT + 1) * sizeof(seq_t));
    if (!fact_counts) exit(1);
    fact_counts[0] = 0;      /* unused */
    fact_counts[1] = 1;      /* definition: one factorization of 1 */

    for (idx_t num = 2; num <= SIEVE_LIMIT; ++num) {
        seq_t sum = 0;
        for (idx_t d = 2; d * d <= num; ++d) {
            if (num % d == 0) {
                /* first factor is d, remaining product is num/d */
                sum += fact_counts[num / d];
                /* also consider first factor as num/d, only if different */
                if (d != num / d) {
                    sum += fact_counts[d];
                }
            }
        }
        /* factorization where the first factor is the number itself */
        sum += 1;
        fact_counts[num] = sum;
    }

    /* we no longer need the prime sieve array */
    free(is_prime);
    initialized = true;
}

seq_t a(idx_t n)
{
    if (!initialized) init();

    if (n == 0) return 0;                /* undefined for 0, return 0 */
    if ((size_t)(n - 1) >= nonprime_cnt)  /* too large for precomputed range */
        return 0;

    idx_t m = nonprimes[n - 1];          /* n-th nonprime number */
    return fact_counts[m];
}
