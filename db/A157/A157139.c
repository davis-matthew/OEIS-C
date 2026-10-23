/*
 * A157139  Odd numbers n with equal number of odd digits in n and n-th prime.
 * https://oeis.org/A157139
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   count_odd_digits_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A157139:
 * a(n) = the n‑th odd integer m such that the number of odd decimal
 * digits in m equals the number of odd digits in the m‑th prime.
 * The term is searched by scanning odd numbers in order, counting
 * odd digits, and comparing with the digit count of the corresponding
 * prime.  Results are cached to allow arbitrary access order.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/count_odd_digits_uint64.h"

/* ---------- primes generation (by incremental trial division) ---------- */
static uint32_t *primes  = NULL;   /* array of primes found so far */
static size_t   primes_len = 0;    /* number of primes stored            */
static size_t   primes_cap = 0;    /* current allocation capacity        */
static uint32_t last_checked = 2;  /* last integer checked for primality */

/* Add a prime to the array, growing storage if needed. */
static void add_prime(uint32_t p)
{
    if (primes_len == primes_cap) {
        size_t new_cap = primes_cap ? primes_cap * 2 : 16;
        uint32_t *tmp = (uint32_t *)realloc(primes, new_cap * sizeof(uint32_t));
        if (!tmp) abort();            /* allocation failure */
        primes      = tmp;
        primes_cap  = new_cap;
    }
    primes[primes_len++] = p;
}

/* Test whether n is prime by dividing by known primes up to sqrt(n). */
static bool is_prime_trial(uint32_t n)
{
    if (n < 2) return false;
    if (n == 2 || n == 3) return true;
    if (n % 2 == 0) return false;
    for (size_t i = 0; i < primes_len; ++i) {
        uint32_t p = primes[i];
        if ((uint64_t)p * p > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* Ensure that the primes array contains at least 'need' primes
 * (i.e., primes[0] is the first prime). */
static void ensure_primes(idx_t need)
{
    if (need <= (idx_t)primes_len) return;

    /* Seed with the first prime if the table is empty. */
    if (primes_len == 0) {
        add_prime(2);
    }

    while (primes_len < (size_t)need) {
        ++last_checked;
        if (is_prime_trial(last_checked)) {
            add_prime(last_checked);
        }
    }
}

/* ---------- sequence cache -------------------- */
static seq_t *cache      = NULL;   /* stored a(n) terms in order */
static size_t cache_len   = 0;     /* number of terms cached    */
static size_t cache_cap   = 0;     /* allocated capacity        */
static idx_t  cur_candidate = 1;   /* last odd number tried     */

/* Grow cache storage to accommodate 'need' elements. */
static void ensure_cache_capacity(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < need) new_cap = need;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                /* allocation failure */
    cache      = tmp;
    cache_cap  = new_cap;
}

/* ---------- public sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* offset behaviour irrelevant */

    /* Build the sequence until we have at least n terms. */
    while (cache_len < (size_t)n) {
        cur_candidate += 2;            /* next odd integer */
        uint64_t cand = (uint64_t)cur_candidate;

        unsigned odd_cand = count_odd_digits_uint64(cand);

        /* Ensure we have the cand‑th prime available. */
        ensure_primes((idx_t)cand);
        uint32_t prime_val = primes[cand - 1];          /* primes[0] = 2 */
        unsigned odd_prime = count_odd_digits_uint64((uint64_t)prime_val);

        if (odd_cand == odd_prime) {
            ensure_cache_capacity(cache_len + 1);
            cache[cache_len++] = (seq_t)cand;
        }
    }
    return cache[n - 1];
}
