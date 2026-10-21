/*
 * A060371  a(n) = (prime(n) - 1)! + 1.
 * https://oeis.org/A060371
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (prime(n) - 1)! + 1 (offset = 1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* The integer types seq_t and idx_t are provided by the build. */

static seq_t *terms   = NULL; /* cached a(1)…a(k) */
static size_t terms_cap   = 0;
static idx_t  terms_len   = 0; /* number of computed terms */

static uint64_t *primes  = NULL; /* list of primes found so far */
static size_t   prime_cap = 0;
static idx_t    prime_cnt = 0;   /* number of primes stored */
static uint64_t  last_prime = 1; /* last prime in the table */

/* current factorial value: fact = (fact_m)! */
static seq_t  fact   = 1;
static idx_t  fact_m = 0;

/* Ensure that terms has capacity at least 'need'. */
static void grow_terms(size_t need)
{
    if (need <= terms_cap)
        return;
    size_t newcap = terms_cap ? terms_cap << 1 : 4;
    while (newcap < need)
        newcap <<= 1;
    terms = (seq_t *)realloc(terms, newcap * sizeof(seq_t));
    terms_cap = newcap;
}

/* Ensure that primes has capacity at least 'need'. */
static void grow_primes(size_t need)
{
    if (need <= prime_cap)
        return;
    size_t newcap = prime_cap ? prime_cap << 1 : 8;
    while (newcap < need)
        newcap <<= 1;
    primes = (uint64_t *)realloc(primes, newcap * sizeof(uint64_t));
    prime_cap = newcap;
}

/* Simple primality test using the primes already found. */
static bool is_prime_candidate(uint64_t num)
{
    if (num < 2)
        return false;
    for (idx_t i = 0; i < prime_cnt; ++i)
    {
        uint64_t p = primes[i];
        if (p * p > num)
            break;
        if (num % p == 0)
            return false;
    }
    return true;
}

/* Expand the cached data up to the requested index 'n'. */
static void ensure_up_to(idx_t n)
{
    while (terms_len < n)
    {
        /* Find the next prime number greater than last_prime. */
        uint64_t cand = last_prime + 1;
        while (!is_prime_candidate(cand))
            ++cand;

        /* Store this prime. */
        grow_primes(prime_cnt + 1);
        primes[prime_cnt++] = cand;
        last_prime = cand;

        /* Extend the running factorial up to (cand - 1)! */
        idx_t target = cand - 1;
        while (fact_m < target)
        {
            ++fact_m;
            fact *= (seq_t)fact_m;
        }

        /* Compute the requested term: (prime - 1)! + 1 */
        seq_t term = fact + (seq_t)1;

        grow_terms(terms_len + 1);
        terms[terms_len++] = term;
    }
}

/* Public sequence routine. */
seq_t a(idx_t n)
{
    if (n == 0)       /* undefined for offset 1 – return a harmless value */
        return (seq_t)0;
    ensure_up_to(n);
    return terms[n - 1];
}
