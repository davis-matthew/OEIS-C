/*
 * A098983  Number of ways of writing n as a sum of a prime and a squarefree
 *          number.
 * https://oeis.org/A098983
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = #{ primes p ≤ n such that n−p is a positive square‑free
 *            integer }
 * Range:     u8 n <= 1709, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (square‑free integers are defined to be positive, hence n−p must be >0) */

#include <stdint.h>
#include <stdlib.h>

/* forward declaration of helper */
static void ensure_limit(idx_t n);

/* static tables */
static unsigned char *is_prime   = NULL;   /* 1 = prime, 0 = composite */
static unsigned char *is_square  = NULL;   /* 1 = square‑free, 0 = not */
static idx_t        *prime_list = NULL;   /* prime values */
static size_t        prime_cnt  = 0;       /* number of primes stored */
static idx_t          sieve_limit = 0;     /* largest n for which tables are valid */

/* --------------------------------------------------------------------- */
static void ensure_limit(idx_t n)
{
    if (n <= sieve_limit)
        return;                         /* tables already up to date   */

    /* allocate new tables for size n+1  (index 0 … n) */
    idx_t new_limit = n;
    unsigned char *new_prime = (unsigned char *)malloc((new_limit + 1) * sizeof(unsigned char));
    unsigned char *new_sf    = (unsigned char *)malloc((new_limit + 1) * sizeof(unsigned char));
    if (!new_prime || !new_sf) {        /* out of memory – bail out */
        if (new_prime) free(new_prime);
        if (new_sf)    free(new_sf);
        return;
    }

    /* initialise all entries to “true” */
    for (idx_t i = 0; i <= new_limit; ++i) {
        new_prime[i] = 1;
        new_sf[i]    = 1;
    }
    new_prime[0] = new_prime[1] = 0;     /* 0 and 1 are not prime          */

    /* Eratosthenes sieve */
    for (idx_t i = 2; i * i <= new_limit; ++i) {
        if (new_prime[i]) {
            for (idx_t j = i * i; j <= new_limit; j += i)
                new_prime[j] = 0;
        }
    }

    /* mark non–square‑free numbers (multiples of prime^2) */
    for (idx_t p = 2; p * p <= new_limit; ++p) {
        if (new_prime[p]) {
            idx_t sq = p * p;
            for (idx_t k = sq; k <= new_limit; k += sq)
                new_sf[k] = 0;
        }
    }

    /* build the prime list for this range */
    size_t cnt = 0;
    for (idx_t i = 2; i <= new_limit; ++i)
        if (new_prime[i]) ++cnt;

    idx_t *new_pr = (idx_t *)malloc(cnt * sizeof(idx_t));
    if (!new_pr) {                /* allocation failed */
        free(new_prime);
        free(new_sf);
        return;
    }
    size_t pos = 0;
    for (idx_t i = 2; i <= new_limit; ++i)
        if (new_prime[i]) new_pr[pos++] = i;

    /* swap old tables with new ones */
    free(is_prime);
    free(is_square);
    free(prime_list);

    is_prime    = new_prime;
    is_square   = new_sf;
    prime_list  = new_pr;
    prime_cnt   = cnt;
    sieve_limit = new_limit;
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* a(0)=a(1)=a(2)=0 by definition (see OEIS listing) */
    if (n < 3)
        return (seq_t)0;

    ensure_limit(n);               /* build tables up to n if needed */

    seq_t count = 0;

    /* iterate over all primes ≤ n */
    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p = prime_list[i];
        if (p > n)
            break;
        idx_t s = n - p;                   /* the complementary square‑free part   */
        if (s > 0 && is_square[s])         /* require s > 0                         */
            ++count;
    }

    return count;
}
