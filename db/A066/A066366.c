/*
 * A066366  Primes which are the sum of a prime number of consecutive primes
 *          in a prime number of different ways.
 * https://oeis.org/A066366
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A066366 – primes that are the sum of a prime number of
   consecutive primes in a prime number of different ways.
   The routine generates primes up to a growing limit, computes all
   sums of consecutive primes, counts how many of those sums are
   prime for each starting position, and records the primes that
   have a prime number of such representations.  The terms are cached
   so that subsequent calls are fast.
*/

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* Sequence type and index type are supplied by the build system */
seq_t a(idx_t n);

/* -----------------------------------------------------------------------
   Static state: prime list, term list, counts, current sieve limit.
----------------------------------------------------------------------- */
static uint32_t *primes = NULL;     /* primes up to current limit           */
static size_t   primes_cnt = 0;    /* number of primes                     */

static seq_t *term_list = NULL;    /* sequence terms found so far          */
static size_t term_cnt = 0;        /* number of terms in term_list         */

static idx_t current_limit = 0;    /* sieve limit used in current table    */

/* -----------------------------------------------------------------------
   Helper: quick test for small primes (used for counts and lengths)
----------------------------------------------------------------------- */
static bool is_small_prime(size_t x)
{
    if (x < 2) return false;
    for (size_t d = 2; d * d <= x; ++d)
        if (x % d == 0) return false;
    return true;
}

/* -----------------------------------------------------------------------
   Rebuild the prime table and the sequence up to the given limit.
----------------------------------------------------------------------- */
static void build_tables(idx_t limit)
{
    /* Free old tables if any */
    free(primes);
    free(term_list);
    primes = NULL;
    term_list = NULL;
    primes_cnt = 0;
    term_cnt = 0;
    current_limit = 0;

    size_t lim = (size_t)limit;
    if (lim < 2) return;

    /* 1. Sieve of Eratosthenes: is_prime[0..lim] */
    unsigned char *is_prime = (unsigned char *)malloc(lim + 1);
    if (!is_prime) return;
    memset(is_prime, 1, lim + 1);
    is_prime[0] = 0;
    if (lim >= 1) is_prime[1] = 0;
    for (size_t i = 2; i * i <= lim; ++i)
        if (is_prime[i])
            for (size_t j = i * i; j <= lim; j += i)
                is_prime[j] = 0;

    /* 2. Collect primes */
    for (size_t i = 2; i <= lim; ++i)
        if (is_prime[i]) ++primes_cnt;
    primes = (uint32_t *)malloc(primes_cnt * sizeof(uint32_t));
    if (!primes) { free(is_prime); return; }
    size_t idx = 0;
    for (size_t i = 2; i <= lim; ++i)
        if (is_prime[i]) primes[idx++] = (uint32_t)i;

    /* 3. Pre‑compute primality of lengths up to primes_cnt */
    size_t max_len = primes_cnt;
    bool *prime_len = (bool *)calloc(max_len + 1, sizeof(bool));
    if (!prime_len) { free(is_prime); free(primes); return; }
    for (size_t i = 0; i <= max_len; ++i) prime_len[i] = true;
    if (max_len >= 0) prime_len[0] = false;
    if (max_len >= 1) prime_len[1] = false;
    for (size_t i = 2; i * i <= max_len; ++i)
        if (prime_len[i])
            for (size_t j = i * i; j <= max_len; j += i)
                prime_len[j] = false;

    /* 4. Count representations: rep[p] = # of prime‑length sums that equal p */
    uint32_t *rep = (uint32_t *)calloc(lim + 1, sizeof(uint32_t));
    if (!rep) { free(is_prime); free(primes); free(prime_len); return; }

    size_t P = primes_cnt;
    for (size_t start = 0; start < P; ++start) {
        uint64_t sum = 0;
        for (size_t offset = 0; offset < P - start; ++offset) {
            sum += primes[start + offset];
            if (sum > lim) break;
            size_t len = offset + 1;
            if (prime_len[len] && is_prime[(size_t)sum])
                ++rep[(size_t)sum];
        }
    }

    /* 5. Collect terms: primes with a prime number of representations */
    for (size_t i = 0; i < P; ++i) {
        uint32_t p = primes[i];
        uint32_t c = rep[p];
        if (c >= 2 && is_small_prime(c)) ++term_cnt;
    }

    term_list = (seq_t *)malloc(term_cnt * sizeof(seq_t));
    if (!term_list) { free(is_prime); free(primes); free(prime_len); free(rep); return; }

    size_t pos = 0;
    for (size_t i = 0; i < P; ++i) {
        uint32_t p = primes[i];
        uint32_t c = rep[p];
        if (c >= 2 && is_small_prime(c))
            term_list[pos++] = (seq_t)p;
    }

    /* 6. Clean up */
    free(is_prime);
    free(prime_len);
    free(rep);
    current_limit = limit;
}

/* -----------------------------------------------------------------------
   Public routine – returns the nth term (n ≥ 1).  The terms are cached.
----------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* Ensure enough terms are available */
    while (term_cnt < (size_t)n) {
        if (current_limit == 0)                 /* first query */
            build_tables(1000000);              /* 1 M is sufficient for early terms */
        else
            build_tables(current_limit * 2);    /* double the limit and rebuild */
    }

    return term_list[n - 1];
}
