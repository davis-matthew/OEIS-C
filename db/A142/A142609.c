/*
 * A142609  Primes congruent to 12 mod 55.
 * https://oeis.org/A142609
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Primes congruent to 12 mod 55
 * Range:     u8 n <= 1, u16 n <= 155, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

/* Sequence type aliases are provided by the build system */
extern seq_t a(idx_t n);

/* ---------- internal state for memoisation ---------- */
static uint64_t *prime_uint64 = NULL;   /* array of found primes */
static size_t     prime_cap   = 0;     /* current capacity */
static size_t     prime_len   = 0;     /* number of stored primes */
static idx_t      max_sieved_limit = 0;/* last limit that was sieved */

/* grow the prime array when it becomes full */
static void grow_prime_array(void)
{
    size_t new_cap = (prime_cap == 0 ? 16 : prime_cap << 1);
    uint64_t *tmp = (uint64_t *)realloc(prime_uint64, new_cap * sizeof(uint64_t));
    if (!tmp) abort();
    prime_uint64 = tmp;
    prime_cap = new_cap;
}

/* perform a sieve of Eratosthenes up to 'limit' and collect
   primes p with p % 55 == 12.  The result replaces the current
   prime list. */
static void sieve_and_collect(idx_t limit)
{
    /* start a fresh list */
    prime_len = 0;

    /* allocate a boolean array: 0 = prime candidate, 1 = composite.
       The array size is limit+1 to keep indices directly usable. */
    unsigned char *comp = (unsigned char *)malloc((size_t)(limit + 1));
    if (!comp) abort();
    memset(comp, 0, (size_t)(limit + 1));

    /* classic sieve: avoid overflow by testing i <= limit / i */
    for (idx_t i = 2; i <= limit / i; ++i) {
        if (!comp[i]) {
            for (idx_t j = i * i; j <= limit; j += i) {
                comp[j] = 1;
            }
        }
    }

    /* collect the wanted primes */
    for (idx_t i = 2; i <= limit; ++i) {
        if (!comp[i] && (i % 55) == 12) {
            if (prime_len == prime_cap) grow_prime_array();
            prime_uint64[prime_len++] = (uint64_t)i;
        }
    }

    free(comp);
    max_sieved_limit = limit;
}

/* ensure that we have at least 'n' terms stored */
static void ensure_terms(idx_t n)
{
    if (prime_len >= n) return;          /* already enough */

    /* start with a modest limit if nothing was sieved yet */
    idx_t limit = (max_sieved_limit == 0) ? 200 : max_sieved_limit;

    /* increase the limit until we have at least n primes.  Each
       iteration recomputes the sieve from scratch, which is
       inexpensive for n <= 10 000. */
    while (prime_len < n) {
        if (limit < 2000000) {               /* simple double strategy */
            limit <<= 1;
        } else {
            limit += limit / 2;               /* increase by 50% for large limits */
        }
        sieve_and_collect(limit);
    }
}

/* public method required by the test harness */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for the OEIS offset */
    ensure_terms(n);
    return (seq_t)prime_uint64[n - 1];
}
