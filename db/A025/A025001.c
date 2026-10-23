/*
 * A025001  a(1) = 5; a(n+1) = a(n)-th nonprime, where nonprimes begin at 0.
 * https://oeis.org/A025001
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 5; a(n+1) = the (a(n)-1)-th nonprime number where
 *            nonprime numbers are listed in ascending order starting with 0.
 *            (Indices in the nonprime list are 0‑based.)
 * Range:     u8 n <= 13, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static const size_t SIEVE_LIMIT = 20000000;   /* upper bound for primes */

/* Stores the (0‑based) nonprime list: nonprime[i] is the i‑th nonprime */
static uint32_t *nonprimes = NULL;
static size_t nonprime_count = 0;

/* Cache of computed a(n) values */
static seq_t *a_cache = NULL;
static size_t a_cache_len = 0;

/* Forward declaration of helper that builds the nonprime list */
static void build_nonprimes(void);

/* Build a list of all nonprime numbers up to SIEVE_LIMIT. */
static void build_nonprimes(void)
{
    /* Sieve of Eratosthenes */
    char *is_prime = (char*)malloc((SIEVE_LIMIT + 1) * sizeof(char));
    for (size_t i = 0; i <= SIEVE_LIMIT; ++i) is_prime[i] = 1;
    if (SIEVE_LIMIT >= 0) is_prime[0] = 0;
    if (SIEVE_LIMIT >= 1) is_prime[1] = 0;
    for (size_t i = 2; i * i <= SIEVE_LIMIT; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= SIEVE_LIMIT; j += i) {
                is_prime[j] = 0;
            }
        }
    }

    /* Count nonprimes to allocate array */
    nonprime_count = 0;
    for (size_t i = 0; i <= SIEVE_LIMIT; ++i) {
        if (!is_prime[i]) ++nonprime_count;
    }

    nonprimes = (uint32_t*)malloc(nonprime_count * sizeof(uint32_t));
    size_t idx = 0;
    for (size_t i = 0; i <= SIEVE_LIMIT; ++i) {
        if (!is_prime[i]) {
            nonprimes[idx++] = (uint32_t)i;
        }
    }

    /* Free sieve to save memory */
    free(is_prime);
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* unused, but safe */

    /* Initialize on first call */
    static bool initialized = false;
    if (!initialized) {
        build_nonprimes();
        initialized = true;
    }

    /* Grow cache to at least n entries */
    if (a_cache_len < (size_t)n) {
        size_t new_len = (size_t)n;
        a_cache = (seq_t*)realloc(a_cache, new_len * sizeof(seq_t));
        for (size_t i = a_cache_len; i < new_len; ++i) {
            if (i == 0) {
                a_cache[0] = (seq_t)5;   /* a(1) */
            } else {
                seq_t prev = a_cache[i - 1];
                /* Convert previous term to 0‑based index in nonprime list */
                if (prev == 0) return (seq_t)0;  /* safety */
                size_t idx = (size_t)(prev - 1); /* idx < nonprime_count guaranteed */
                a_cache[i] = (seq_t)nonprimes[idx];
            }
        }
        a_cache_len = new_len;
    }

    return a_cache[n - 1];
}
