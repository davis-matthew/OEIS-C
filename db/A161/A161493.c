/*
 * A161493  Positive integers, k, for which k mod d(k) and k have opposite
 *          (odd/even) parity, where d(k) is the number of divisors of k.
 * https://oeis.org/A161493
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 109, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for squares m² that satisfy ⌊m² / d(m²)⌋ odd.  The function
   a(n) returns the n‑th such square (OEIS A161493). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

/* ----- static cache for the sequence ----- */
static seq_t *cache      = NULL;  /* cached terms a(1)..a(max) */
static idx_t  cache_count = 0;    /* number of cached terms   */
static idx_t  cache_cap   = 0;    /* current capacity         */

/* ----- static list of primes used for factorisation ----- */
static uint32_t *prime_list = NULL;   /* primes up to current limit */
static size_t   prime_cnt   = 0;      /* number of primes stored   */
static size_t   prime_cap   = 0;      /* current capacity of list  */

/* ----- last candidate m processed ----- */
static idx_t last_m = 0;

/*--------------------------------------------------------------*/
/* ensure prime_list contains all primes <= limit (limit >= 2) */
/*--------------------------------------------------------------*/
static void ensure_primes(size_t limit)
{
    if (limit <= prime_cap)
        return;                       /* already sufficient */

    bool *is_comp = (bool *)calloc(limit + 1, sizeof(bool));
    if (!is_comp)
        return;                       /* out of memory – abort silently */

    free(prime_list);
    prime_list = NULL;
    prime_cnt  = 0;
    prime_cap  = 0;

    /* sieve of Eratosthenes */
    for (size_t i = 2; i <= limit; ++i)
        is_comp[i] = false;            /* assume prime */

    for (size_t i = 2; i * i <= limit; ++i) {
        if (!is_comp[i]) {
            for (size_t j = i * i; j <= limit; j += i)
                is_comp[j] = true;
        }
    }

    for (size_t i = 2; i <= limit; ++i) {
        if (!is_comp[i]) {
            prime_list = (uint32_t *)realloc(
                prime_list, (prime_cnt + 1) * sizeof(uint32_t));
            if (!prime_list) return;   /* out of memory – abort silently */
            prime_list[prime_cnt++] = (uint32_t)i;
        }
    }

    free(is_comp);
    prime_cap = limit;
}

/*--------------------------------------------------------------*/
/* push a term into the sequence cache, expanding it if needed  */
/*--------------------------------------------------------------*/
static void cache_push(seq_t val)
{
    if (cache_count == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *new_cache = (seq_t *)realloc(
            cache, new_cap * sizeof(seq_t));
        if (!new_cache) return;       /* out of memory – abort silently */
        cache      = new_cache;
        cache_cap  = new_cap;
    }
    cache[cache_count++] = val;
}

/*--------------------------------------------------------------*/
/* main sequence function                                         */
/*--------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* undefined, return 0 */

    /* extend cache until it contains at least n terms */
    while (cache_count < n) {
        ++last_m;                     /* next m to test */

        /* ensure we have primes up to sqrt(last_m) */
        size_t limit = (size_t)(sqrt((long double)last_m) + 1.0);
        if (limit < 2)
            limit = 2;
        ensure_primes(limit);

        /* compute d(m²) where m = last_m */
        seq_t d = 1;
        uint64_t tmp = last_m;
        for (size_t i = 0; i < prime_cnt &&
              (uint64_t)prime_list[i] * prime_list[i] <= tmp; ++i) {
            uint32_t p = prime_list[i];
            if (tmp % p == 0) {
                unsigned exp = 0;
                while (tmp % p == 0) {
                    tmp /= p;
                    ++exp;
                }
                d *= (seq_t)(2 * exp + 1);   /* contributes (2e+1) */
            }
        }
        if (tmp > 1) {
            /* remaining prime factor with exponent 1 */
            d *= (seq_t)3;                  /* 2*1+1 = 3 */
        }

        /* k = m² and q = floor(k / d) */
        seq_t k = (seq_t)last_m * (seq_t)last_m;
        seq_t q = k / d;

        /* qualify if q is odd */
        if ((q & (seq_t)1) != 0)
            cache_push(k);
    }

    /* sequences are stored 0‑based, index n-1 holds a(n) */
    return cache[n - 1];
}
