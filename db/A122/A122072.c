/*
 * A122072  Greatest prime less than 10*n.
 * https://oeis.org/A122072
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 6553, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the greatest prime less than 10*n (A122072) */
#include <stdlib.h>
#include <stdbool.h>

/* static cache of primes up to the largest target seen so far */
static uint64_t *prime_arr = NULL;   /* array of primes */
static size_t prime_cnt = 0;         /* number of primes stored */
static idx_t sieve_limit = 0;        /* maximum number for which sieve has been built */

/* build primes up to 'limit' (inclusive) using a simple sieve */
static void build_sieve_upto(idx_t limit)
{
    /* allocate boolean array for primality flags */
    bool *is_prime = (bool *)calloc((size_t)(limit + 1), sizeof(bool));
    if (!is_prime) return;

    /* initialize all entries to true, then clear 0 and 1 */
    for (idx_t i = 0; i <= limit; ++i) is_prime[i] = true;
    if (limit >= 0) is_prime[0] = false;
    if (limit >= 1) is_prime[1] = false;

    /* classic Eratosthenes sieve */
    for (idx_t p = 2; p * p <= limit; ++p) {
        if (is_prime[p]) {
            for (idx_t m = p * p; m <= limit; m += p)
                is_prime[m] = false;
        }
    }

    /* count primes */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++cnt;

    /* store primes in a new array */
    uint64_t *pr = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!pr) {
        free(is_prime);
        return;
    }

    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) pr[idx++] = (uint64_t)i;

    /* replace old cache */
    free(prime_arr);
    prime_arr = pr;
    prime_cnt = cnt;
    sieve_limit = limit;

    free(is_prime);
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* out of range */

    /* target is 10 * n, we need primes strictly less than this */
    idx_t target = (idx_t)10 * n;
    idx_t needed_limit = target - 1;

    /* extend sieve if necessary */
    if (sieve_limit < needed_limit)
        build_sieve_upto(needed_limit);

    /* search for the largest prime < target */
    size_t i = prime_cnt;
    while (i > 0 && prime_arr[i - 1] >= target) --i;

    if (i == 0) return (seq_t)0;          /* should not happen for n >= 1 */
    return (seq_t)prime_arr[i - 1];
}
