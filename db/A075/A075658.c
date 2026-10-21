/*
 * A075658  Numbers k such that the sum of prime divisors of k (A008472) is
 *          composite.
 * https://oeis.org/A075658
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 142, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes numbers k such that the sum of distinct prime factors of k
   is a composite number.  The function generates these numbers by
   sieving integers and selecting those whose prime‑factor sum is
   composite. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Primality test for the small sums occurring in this sequence. */
static bool is_prime_small(uint64_t n)
{
    if (n < 2)           return false;
    if (n % 2 == 0)      return n == 2;
    for (uint64_t i = 3; i * i <= n; i += 2)
        if (n % i == 0)  return false;
    return true;
}

/* ----------  Global state for the sieve  ---------- */
static idx_t    max_sieve = 0;          /* highest index currently sieved   */
static idx_t *  spf       = NULL;       /* smallest prime factor of n       */
static seq_t *  sum_dist  = NULL;       /* sum of distinct prime factors    */
static idx_t *  primes    = NULL;       /* list of primes – linear sieve    */
static size_t  primes_cap = 0;
static size_t  primes_len = 0;

/* Sequence storage */
static seq_t * terms   = NULL;
static idx_t   terms_len = 0;
static size_t  terms_cap = 0;

/* Extend the sieve so that all numbers up to 'limit' are sieved.
   The function keeps the previously computed data and only
   initializes the new region. */
static void extend_sieve(idx_t limit)
{
    if (limit <= max_sieve) return;

    size_t oldsize = (size_t)max_sieve + 1;
    size_t newsize = (size_t)limit + 1;

    idx_t *new_spf    = (idx_t *)realloc(spf,   newsize * sizeof(idx_t));
    if (!new_spf) exit(EXIT_FAILURE);
    sum_dist = (seq_t *)realloc(sum_dist, newsize * sizeof(seq_t));
    if (!sum_dist) exit(EXIT_FAILURE);

    /* Zero the new portion of the spf array */
    for (size_t i = oldsize; i < newsize; ++i) new_spf[i] = 0;
    spf = new_spf;

    /* Ensure the primes array is large enough */
    if (primes_cap < newsize + 10) {
        size_t new_cap = newsize + 20;
        idx_t *new_primes = (idx_t *)realloc(primes, new_cap * sizeof(idx_t));
        if (!new_primes) exit(EXIT_FAILURE);
        primes = new_primes;
        primes_cap = new_cap;
    }

    /* Linear sieve to fill spf up to 'limit'. */
    for (idx_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            if (primes_len == primes_cap) {
                size_t new_cap = primes_cap * 2;
                idx_t *tmp = (idx_t *)realloc(primes, new_cap * sizeof(idx_t));
                if (!tmp) exit(EXIT_FAILURE);
                primes = tmp;
                primes_cap = new_cap;
            }
            primes[primes_len++] = i;
        }
        for (size_t j = 0; j < primes_len && primes[j] <= spf[i]; ++j) {
            idx_t p  = primes[j];
            idx_t ip = i * p;
            if (ip > limit) break;
            if (spf[ip] == 0) spf[ip] = p;
        }
    }

    /* Initialise sums for 0 and 1 once. */
    if (max_sieve < 1) {
        sum_dist[0] = 0;
        sum_dist[1] = 0;
    }

    max_sieve = limit;
}

/* The public function required by the framework. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, return 0 for safety */

    static idx_t next_k = 2;        /* next integer to process */

    while (terms_len < n) {
        /* Ensure we have data up to next_k. */
        if (next_k > max_sieve) {
            idx_t new_limit = max_sieve ? 2 * max_sieve : 100;
            if (new_limit < next_k) new_limit = next_k;
            extend_sieve(new_limit);
        }

        /* Compute sum of distinct prime factors for k. */
        idx_t  k    = next_k;
        idx_t  p    = spf[k];
        idx_t  m    = k / p;
        seq_t  sum  = sum_dist[m];
        if (m % p != 0) sum += (seq_t)p;   /* add p only once */
        sum_dist[k] = sum;

        /* Test if the sum is composite (greater than 1 and not prime). */
        bool is_comp = false;
        if (sum > 1) {
            uint64_t su = (uint64_t)sum;
            if (!is_prime_small(su)) is_comp = true;
        }

        if (is_comp) {
            if (terms_len == terms_cap) {
                terms_cap = terms_cap ? terms_cap * 2 : 4;
                seq_t *tmp = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
                if (!tmp) exit(EXIT_FAILURE);
                terms = tmp;
            }
            terms[terms_len++] = (seq_t)k;
        }

        ++next_k;
    }

    return terms[n - 1];
}
