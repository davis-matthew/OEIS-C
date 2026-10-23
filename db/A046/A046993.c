/*
 * A046993  Partial products of pi(n), A000720.
 * https://oeis.org/A046993
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Π_{k=2}^{n-1} π(k) -- partial products of pi(n) (A046993)
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 18, u64 n <= 28, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* -------------------------------------------------------------------- */
/* Prime sieve – keeps the primes up to a current limit                 */
/* -------------------------------------------------------------------- */

static idx_t    sieve_limit     = 0;
static bool   * sieve_is_comp  = NULL;   /* composite flags */
static uint64_t * sieve_primes  = NULL;   /* list of primes */
static size_t   sieve_prime_cnt = 0;       /* count of primes */

/* Allocate or extend the sieve so that it covers all numbers ≤ limit   */
static void generate_primes_upto(idx_t limit)
{
    if (limit <= sieve_limit)               /* already covered */
        return;

    /* free old data */
    free(sieve_is_comp);
    free(sieve_primes);
    sieve_is_comp  = NULL;
    sieve_primes   = NULL;
    sieve_limit    = 0;
    sieve_prime_cnt= 0;

    /* ------------------------------------------------- sieve of Eratosthenes */
    sieve_is_comp = (bool*)calloc((size_t)(limit + 1), sizeof(bool));
    if (!sieve_is_comp) return;             /* allocation failure */

    for (idx_t i = 2; (i * i) <= limit; ++i) {
        if (!sieve_is_comp[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                sieve_is_comp[j] = true;
        }
    }

    /* Count primes */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!sieve_is_comp[i]) ++cnt;

    /* Store primes */
    sieve_primes = (uint64_t*)malloc(cnt * sizeof(uint64_t));
    if (!sieve_primes) { free(sieve_is_comp); sieve_is_comp = NULL; return; }

    size_t pos = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!sieve_is_comp[i]) sieve_primes[pos++] = i;

    sieve_prime_cnt = cnt;
    sieve_limit     = limit;
}

/* -------------------------------------------------------------------- */
/* Cache for a(n) values                                                */
/* -------------------------------------------------------------------- */

static seq_t * a_cache      = NULL;   /* a_cache[i] holds a(i)   */
static idx_t   a_cache_cap  = 0;       /* allocated size         */
static idx_t   a_cache_last = 0;       /* largest index computed */

/* State for incremental prime counting while iterating over n        */
static idx_t   prime_idx = 0;          /* next prime index in sieve_primes */
static idx_t   prime_pi  = 0;          /* current value of π(i-1)          */

seq_t a(idx_t n)
{
    if (n < 2) return 0;                 /* undefined below offset 2 */

    /* ---------------------------------------------------------------- */
    /* 1. Initialise cache on first call                              */
    /* ---------------------------------------------------------------- */
    if (!a_cache) {
        size_t init_cap = (n > 2) ? (size_t)(n + 1) : 3; /* space for 0..2 at least */
        a_cache = (seq_t*)malloc(init_cap * sizeof(seq_t));
        if (!a_cache) return 0;          /* allocation failure */

        a_cache[0] = 0;
        a_cache[1] = 0;
        a_cache[2] = 1;                 /* a(2) = 1   */
        a_cache_cap = init_cap;
        a_cache_last = 2;

        /* initialise prime counting state */
        prime_idx = 0;
        prime_pi  = 0;
    }

    /* ---------------------------------------------------------------- */
    /* 2. Grow cache if requested index is beyond current capacity     */
    /* ---------------------------------------------------------------- */
    if (n >= a_cache_cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t*)realloc(a_cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;              /* reallocation failed */
        a_cache = tmp;
        a_cache_cap = new_cap;
    }

    /* ---------------------------------------------------------------- */
    /* 3. Compute new entries up to n if needed                         */
    /* ---------------------------------------------------------------- */
    if (n > a_cache_last) {
        /* Ensure enough primes are available */
        generate_primes_upto(n - 1);

        for (idx_t i = a_cache_last + 1; i <= n; ++i) {

            /* Count primes ≤ i-1 by scanning the sieve */
            while (prime_idx < sieve_prime_cnt &&
                   sieve_primes[prime_idx] <= i - 1) {
                ++prime_idx;
                ++prime_pi;
            }

            /* a(i) = a(i-1) * π(i-1) */
            a_cache[i] = a_cache[i - 1] * (seq_t)prime_pi;
        }
        a_cache_last = n;
    }

    return a_cache[n];
}
