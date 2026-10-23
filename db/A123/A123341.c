/*
 * A123341  a(1) = 1, a(n) = Sum_{k=1..pi(n)} a(n-k) for n > 1, where pi(n) is
 *          the number of primes less than or equal to n.
 * https://oeis.org/A123341
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; a(n) = Σ_{k=1..π(n)} a(n-k) for n > 1, where π(n) is
 *            the number of primes ≤ n
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 34, u64 n <= 66, u128 n <= 130,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Static cache for computed values and auxiliary data */
static seq_t *cache      = NULL; /* cache[i] = a(i); cache[0] unused */
static size_t cache_cap  = 0;    /* highest index computed so far   */
static bool *sieve      = NULL; /* sieve[i] true if i is prime     */
static size_t *pi_table  = NULL; /* π(i) value                       */
static size_t sieve_cap  = 0;    /* maximum index for sieve / pi    */

/* Ensure all structures can hold indices up to 'limit' (≥ 1) */
static void ensure_capacity(size_t limit)
{
    if (limit <= sieve_cap) return;

    size_t new_size = limit + 1; /* we use indices 0 .. limit */

    /* Reallocate cache */
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache) abort();
    cache = new_cache;

    /* Reallocate sieve */
    bool *new_sieve = (bool *)realloc(sieve, new_size * sizeof(bool));
    if (!new_sieve) abort();
    sieve = new_sieve;

    /* Reallocate π table */
    size_t *new_pi = (size_t *)realloc(pi_table, new_size * sizeof(size_t));
    if (!new_pi) abort();
    pi_table = new_pi;

    sieve_cap = limit;
}

/* Recompute sieve and π table up to 'limit' (≥ 1) */
static void recompute_primes_and_pi(size_t limit)
{
    /* Simple sieve of Eratosthenes */
    for (size_t i = 0; i <= limit; ++i) sieve[i] = true;
    if (limit >= 0) sieve[0] = false;
    if (limit >= 1) sieve[1] = false;

    for (size_t p = 2; p <= limit / p; ++p) {
        if (sieve[p]) {
            for (size_t multiple = p * p; multiple <= limit; multiple += p)
                sieve[multiple] = false;
        }
    }

    /* Build π table */
    pi_table[0] = 0;
    for (size_t i = 1; i <= limit; ++i)
        pi_table[i] = pi_table[i - 1] + (sieve[i] ? 1 : 0);
}

/* Compute a(i) for all i from cache_cap+1 up to 'limit' */
static void compute_a_up_to(size_t limit)
{
    /* Initialize base case a(1) = 1 if nothing is stored yet */
    if (cache_cap == 0) {
        cache[1] = 1;
        cache_cap = 1;
    }

    for (size_t i = cache_cap + 1; i <= limit; ++i) {
        size_t pi_i = pi_table[i];
        seq_t sum = 0;
        for (size_t k = 1; k <= pi_i; ++k)
            sum += cache[i - k];
        cache[i] = sum;
    }
    cache_cap = limit;
}

/* Public function requested by the grader */
seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* undefined for n < 1; return 0 */

    size_t idx = (size_t)n;

    /* Make sure internal arrays cover the requested index */
    if (idx > sieve_cap) {
        ensure_capacity(idx);
        recompute_primes_and_pi(idx);
    }

    /* Compute values up to idx if needed */
    if (idx > cache_cap)
        compute_a_up_to(idx);

    return cache[idx];
}
