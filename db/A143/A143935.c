/*
 * A143935  Number of primes between n^K and (n+1)^K, inclusive, where
 *          K=log(127)/log(16).
 * https://oeis.org/A143935
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = number of primes in the interval [ceil(n^K), floor((n+1)^K)]
   where K = log(127)/log(16). The function computes the result using
   a sieve of Eratosthenes that is dynamically extended to cover the
   required upper bound. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* ---------- constants ---------- */
static const long double K = logl(127.0L) / logl(16.0L);   /* K = log(127)/log(16) */

/* ---------- sieve state ---------- */
static idx_t sieve_limit = 0;           /* current maximum sieved index */
static uint8_t *is_prime   = NULL;      /* 1 = prime, 0 = composite */
static seq_t *prime_pi     = NULL;      /* prefix prime counts */

/* ---------- prime list for incremental sieving ---------- */
static idx_t *primes_list = NULL;       /* array of primes found so far */
static size_t primes_cnt   = 0;
static size_t primes_cap   = 0;

/* ---------- helper: extend sieve up to new_limit ---------- */
static void extend_sieve(idx_t new_limit)
{
    if (new_limit <= sieve_limit)
        return; /* nothing to do */

    /* First time initialization */
    if (sieve_limit == 0) {
        /* allocate arrays for [0 … new_limit] */
        is_prime   = (uint8_t *)malloc((size_t)new_limit + 1);
        prime_pi   = (seq_t *)calloc((size_t)new_limit + 1, sizeof(seq_t));
        if (!is_prime || !prime_pi)
            abort(); /* out of memory */

        /* Mark all as prime initially and then correct 0,1 */
        for (idx_t i = 0; i <= new_limit; ++i)
            is_prime[i] = 1;
        is_prime[0] = is_prime[1] = 0;

        /* Run ordinary sieve up to sqrt(new_limit) */
        idx_t sqrt_lim = (idx_t)sqrtl((long double)new_limit);
        for (idx_t p = 2; p <= sqrt_lim; ++p) {
            if (!is_prime[p]) continue;
            for (idx_t j = p * p; j <= new_limit; j += p)
                is_prime[j] = 0;
        }

        /* Build prime list and prefix counts */
        seq_t count = 0;
        for (idx_t i = 0; i <= new_limit; ++i) {
            if (is_prime[i]) {
                ++count;
                if (primes_cnt == primes_cap) {
                    size_t new_cap = (primes_cap == 0) ? 64 : primes_cap * 2;
                    idx_t *tmp = (idx_t *)realloc(primes_list, new_cap * sizeof(idx_t));
                    if (!tmp) abort();
                    primes_list = tmp;
                    primes_cap = new_cap;
                }
                primes_list[primes_cnt++] = i;
            }
            prime_pi[i] = count;
        }

        sieve_limit = new_limit;
        return;
    }

    /* Incremental extension: enlarge arrays */
    uint8_t *tmp_bytes = (uint8_t *)realloc(is_prime, (size_t)new_limit + 1);
    seq_t *tmp_pi      = (seq_t *)realloc(prime_pi, (size_t)(new_limit + 1) * sizeof(seq_t));
    if (!tmp_bytes || !tmp_pi)
        abort();
    is_prime = tmp_bytes;
    prime_pi = tmp_pi;

    /* Initialize new region as possibly prime */
    for (idx_t i = sieve_limit + 1; i <= new_limit; ++i)
        is_prime[i] = 1;
    if (sieve_limit == 0) {
        if (new_limit >= 0) is_prime[0] = 0;
        if (new_limit >= 1) is_prime[1] = 0;
    }

    /* Sieve composites in the new region using existing primes */
    idx_t sqrt_new = (idx_t)sqrtl((long double)new_limit);
    for (size_t idx = 0; idx < primes_cnt; ++idx) {
        idx_t p = primes_list[idx];
        if (p > sqrt_new)
            break;
        idx_t start = ((sieve_limit + 1 + p - 1) / p) * p;
        if (start < (idx_t)p * p)
            start = (idx_t)p * p;
        for (idx_t j = start; j <= new_limit; j += p)
            is_prime[j] = 0;
    }

    /* Fill prime list and prefix counts for new indices */
    seq_t count = prime_pi[sieve_limit];
    for (idx_t i = sieve_limit + 1; i <= new_limit; ++i) {
        if (is_prime[i]) {
            ++count;
            if (primes_cnt == primes_cap) {
                size_t new_cap = (primes_cap == 0) ? 64 : primes_cap * 2;
                idx_t *tmp = (idx_t *)realloc(primes_list, new_cap * sizeof(idx_t));
                if (!tmp) abort();
                primes_list = tmp;
                primes_cap = new_cap;
            }
            primes_list[primes_cnt++] = i;
        }
        prime_pi[i] = count;
    }

    sieve_limit = new_limit;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* offset starts at 1 */

    /* Compute interval bounds with a tiny epsilon to avoid rounding issues */
    const long double eps = 1e-12L;

    long double low_val  = powl((long double)n,     K);
    long double high_val = powl((long double)(n + 1), K);

    idx_t low  = (idx_t)ceill(low_val  - eps);     /* ceil(n^K)   */
    idx_t high = (idx_t)floorl(high_val + eps);    /* floor((n+1)^K) */

    if (high < low)
        return 0; /* empty interval */

    /* Ensure sieve covers the required upper bound */
    extend_sieve(high);

    seq_t pi_high  = prime_pi[high];
    seq_t pi_lowm1 = (low > 0) ? prime_pi[low - 1] : 0;

    return pi_high - pi_lowm1;
}
