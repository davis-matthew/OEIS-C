/*
 * A130911  a(n) is the number of primes with odd binary weight among the
 *          first n primes minus the number with an even binary weight.
 * https://oeis.org/A130911
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nice, sign, base
 * Formula:   a(n) = (number of odious primes ≤ prime(n)) – (number of evil
 *            primes ≤ prime(n))
 * Range:     i8 n <= 35, i16 n <= 3887, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  (odious = binary weight odd, evil = binary weight even) */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/is_prime_small.h"   /* bool is_prime_small(uint32_t num); */

static seq_t *cum = NULL;        /* cum[i] = a(i), 1‑based; cum[0] unused */
static size_t  cum_size = 0;     /* largest index cached */

static uint32_t *prime_cache = NULL;   /* list of primes found so far */
static size_t   prime_cnt   = 0;
static size_t   prime_cap   = 0;

/* Make sure we have computed a(1)…a(n)  */
static void
ensure_cumulative(idx_t n)
{
    if ((size_t)n <= cum_size)            /* already cached */
        return;

    /* --- grow cum array -------------------------------------------------- */
    size_t new_cum_size = (size_t)n + 1;   /* include index 0 unused */
    seq_t *new_cum = NULL;
    if (cum == NULL) {
        new_cum = (seq_t *)malloc(new_cum_size * sizeof(seq_t));
    } else {
        new_cum = (seq_t *)realloc(cum, new_cum_size * sizeof(seq_t));
    }
    if (new_cum == NULL)
        return;        /* allocation failed – cannot continue */
    cum = new_cum;

    /* --- grow prime_cache array ----------------------------------------- */
    if (prime_cnt == 0) {
        prime_cap = 1024;   /* initial capacity */
        prime_cache = (uint32_t *)malloc(prime_cap * sizeof(uint32_t));
        if (prime_cache == NULL)
            return;        /* allocation failed */
    } else if (prime_cnt >= prime_cap) {
        prime_cap *= 2;
        uint32_t *tmp = (uint32_t *)realloc(prime_cache, prime_cap * sizeof(uint32_t));
        if (tmp == NULL)
            return;        /* allocation failed */
        prime_cache = tmp;
    }

    /* --- compute missing terms ------------------------------------------ */
    seq_t sum = (cum_size ? cum[cum_size] : (seq_t)0);
    uint32_t last_prime = (prime_cnt ? prime_cache[prime_cnt - 1] : 0);

    /* if we have no primes yet, start with 2 */
    if (prime_cnt == 0) {
        last_prime = 2;
        ++prime_cnt;
        if (prime_cnt > prime_cap) {
            prime_cap *= 2;
            uint32_t *tmp = (uint32_t *)realloc(prime_cache, prime_cap * sizeof(uint32_t));
            if (tmp == NULL) return;
            prime_cache = tmp;
        }
        prime_cache[prime_cnt - 1] = last_prime;
        sum += ( (__builtin_popcount(last_prime) & 1U) ? (seq_t)1 : (seq_t)(-1) );
        cum[1] = sum;
    }

    /* generate further primes until we have n of them */
    for (size_t i = prime_cnt; i < (size_t)n; ++i) {
        /* find next prime */
        uint32_t cand = last_prime + 1;
        if (cand <= 2) cand = 3;  /* ensure odd after 2 */
        if ((cand & 1U) == 0) cand++;   /* make odd */

        while (!is_prime_small(cand)) {
            cand += 2;
        }
        last_prime = cand;
        if (prime_cnt >= prime_cap) {
            prime_cap *= 2;
            uint32_t *tmp = (uint32_t *)realloc(prime_cache, prime_cap * sizeof(uint32_t));
            if (tmp == NULL) return;
            prime_cache = tmp;
        }
        prime_cache[prime_cnt++] = last_prime;

        /* update cumulative sum */
        unsigned int pop = __builtin_popcount(last_prime);
        sum += ( (pop & 1U) ? (seq_t)1 : (seq_t)(-1) );
        cum[i + 1] = sum;
    }

    /* update global size */
    cum_size = (size_t)n;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;          /* undefined for n < 1 */
    ensure_cumulative(n);
    return cum[(size_t)n];
}
