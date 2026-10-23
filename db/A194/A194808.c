/*
 * A194808  Twin primes modulo 5.
 * https://oeis.org/A194808
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = n-th prime p such that |p-2| is also prime, modulo 5 */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

static idx_t sieve_limit = 0;            /* current upper bound of the sieve   */
static bool *is_prime      = NULL;       /* sieve marks: true iff prime       */
static seq_t *twin_primes  = NULL;       /* primes that have a twin, sorted   */
static size_t twin_len     = 0;          /* number of primes in the list      */

/* Build a prime sieve up to sieve_limit and fill twin_primes. */
static void
build_sieve(void)
{
    bool *new_is_prime = (bool *)calloc(sieve_limit + 1, sizeof(bool));
    if (!new_is_prime)
        return;   /* memory failure – the caller will use the old data */

    /* Mark all numbers >= 2 as prime initially. */
    for (idx_t i = 2; i <= sieve_limit; ++i)
        new_is_prime[i] = true;

    for (idx_t i = 2; i * i <= sieve_limit; ++i) {
        if (!new_is_prime[i])
            continue;
        for (idx_t j = i * i; j <= sieve_limit; j += i)
            new_is_prime[j] = false;
    }

    /* Estimate needed capacity for twin_primes. */
    size_t est_cap = sieve_limit / 10 + 10;
    seq_t *new_twin = (seq_t *)malloc(est_cap * sizeof(seq_t));
    if (!new_twin) {
        free(new_is_prime);
        return;
    }

    size_t len = 0;
    for (idx_t p = 2; p <= sieve_limit; ++p) {
        if (!new_is_prime[p])
            continue;
        bool has_twin = false;
        if (p >= 3 && new_is_prime[p - 2])   /* lower twin present    */
            has_twin = true;
        else if (p + 2 <= sieve_limit && new_is_prime[p + 2]) /* upper twin */
            has_twin = true;
        if (has_twin) {
            if (len == est_cap) {
                est_cap *= 2;
                new_twin = (seq_t *)realloc(new_twin, est_cap * sizeof(seq_t));
                if (!new_twin)
                    break;   /* out of memory – keep current list      */
            }
            new_twin[len++] = (seq_t)p;
        }
    }

    /* Replace old data. */
    free(is_prime);
    free(twin_primes);
    is_prime     = new_is_prime;
    twin_primes  = new_twin;
    twin_len     = len;
}

/* Ensure at least 'need' twin primes are available. */
static void
ensure_twin_primes(idx_t need)
{
    if (twin_len >= (size_t)need)
        return;

    if (sieve_limit == 0)
        sieve_limit = 100000;   /* initial guess */

    while (twin_len < (size_t)need) {
        build_sieve();
        if (twin_len >= (size_t)need)
            break;
        sieve_limit *= 2;  /* grow the sieve until enough primes are found */
    }
}

seq_t
a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* out of bounds – never used in OEIS data */

    ensure_twin_primes(n);
    if (n - 1 >= twin_len)   /* defensive but should not happen */
        return (seq_t)0;

    seq_t prime = twin_primes[n - 1];
    return prime % (seq_t)5U;
}
