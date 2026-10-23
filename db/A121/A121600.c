/*
 * A121600  a(n) = number of earlier terms which are not divisible by the
 *          largest prime dividing n, for n >= 2, with a(1) = 1.
 * https://oeis.org/A121600
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   largest_prime_divisor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = number of earlier terms that are not divisible
 *              by the largest prime dividing n
 *              (a(1) = 1).  The sequence is indexed starting at 1.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "../utils/largest_prime_divisor.h"  /* gives uint64_t largest_prime_divisor(uint64_t) */

/* --- Global tables --- */
static seq_t *cache                = NULL;   /* cache[n] = a(n), 1‑based */
static size_t cache_size           = 0;      /* allocated cache size (>= n+1) */
static idx_t  max_computed         = 0;      /* highest n already computed */

static seq_t *prime_divcnt         = NULL;   /* prime_divcnt[p] = #prev terms divisible by p */
static size_t prime_divcnt_size    = 0;      /* allocated size (>= maximum prime seen+1) */

static uint64_t *prime_list       = NULL;   /* primes ascending */
static size_t   prime_list_cnt    = 0;       /* number of primes stored */

/* --- Utility: ensure cache big enough --- */
static void ensure_cache_size(size_t need)
{
    if (cache_size >= need) return;
    size_t newsize = cache_size ? cache_size * 2 : 8;
    while (newsize < need) newsize <<= 1;
    cache = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
    if (!cache) abort();
    memset(cache + cache_size, 0, (newsize - cache_size) * sizeof(seq_t));
    cache_size = newsize;
}

/* --- Utility: ensure prime_divcnt big enough --- */
static void ensure_prime_divcnt_size(size_t need)
{
    if (prime_divcnt_size > need) return;   /* > rather than >= to avoid realloc on exact size */
    size_t newsize = prime_divcnt_size ? prime_divcnt_size * 2 : 16;
    while (newsize < need) newsize <<= 1;
    prime_divcnt = (seq_t *)realloc(prime_divcnt, newsize * sizeof(seq_t));
    if (!prime_divcnt) abort();
    memset(prime_divcnt + prime_divcnt_size, 0,
           (newsize - prime_divcnt_size) * sizeof(seq_t));
    prime_divcnt_size = newsize;
}

/* --- Utility: generate prime list up to limit --- */
static void ensure_prime_list(size_t limit)
{
    if (prime_list && prime_list_cnt > 0 &&
        prime_list[prime_list_cnt - 1] >= limit)
        return;   /* already sufficient */

    size_t sz   = limit + 1;
    char  *is_comp = (char *)calloc(sz, 1);
    if (!is_comp) abort();

    size_t need = sz / 10 + 10;          /* approximate number of primes */
    prime_list = (uint64_t *)realloc(prime_list, need * sizeof(uint64_t));
    if (!prime_list) abort();

    size_t count = 0;
    for (size_t i = 2; i <= limit; ++i) {
        if (!is_comp[i]) {
            if (count == need) {
                need <<= 1;
                prime_list = (uint64_t *)realloc(prime_list,
                                                 need * sizeof(uint64_t));
                if (!prime_list) abort();
            }
            prime_list[count++] = i;
            if (i * i <= limit) {
                for (size_t j = i * i; j <= limit; j += i)
                    is_comp[j] = 1;
            }
        }
    }
    free(is_comp);
    prime_list_cnt = count;
}

/* --- Increment count for a prime divisor --- */
static void inc_prime_divcnt(uint64_t prime)
{
    if (prime >= prime_divcnt_size)   /* grow array if needed */
        ensure_prime_divcnt_size((size_t)prime + 1);
    prime_divcnt[prime] += 1;
}

/* --- Helper: factor distinct prime divisors of v --- */
static void factor_distinct_primes(seq_t v)
{
    if (v <= 1) return;
    seq_t rem = v;
    for (size_t i = 0; i < prime_list_cnt; ++i) {
        uint64_t p = prime_list[i];
        if ((uint64_t)p * (uint64_t)p > rem) break;
        if (rem % p == 0) {
            inc_prime_divcnt(p);
            do { rem /= p; } while (rem % p == 0);
        }
    }
    if (rem > 1) inc_prime_divcnt((uint64_t)rem);
}

/* --- Sequence function --- */
seq_t a(idx_t n)
{
    if (n == 1) return 1;          /* offset 1 */

    /* Ensure cache can hold up to n */
    ensure_cache_size((size_t)n + 1);

    /* Initialize a(1) on first call */
    if (max_computed == 0) {
        cache[1] = 1;
        max_computed = 1;
    }

    /* Return cached value if known */
    if (cache[n] != 0) return cache[n];

    /* Ensure prime utilities ready */
    ensure_prime_list((size_t)n);
    ensure_prime_divcnt_size((size_t)n + 1);

    /* Compute missing terms in order */
    for (idx_t curr = max_computed + 1; curr <= n; ++curr) {
        uint64_t p = largest_prime_divisor((uint64_t)curr);
        seq_t notDivisible = (seq_t)(curr - 1) - prime_divcnt[p];
        cache[curr] = notDivisible;

        /* Update counts for the newly generated term if >1 */
        if (notDivisible > 1)
            factor_distinct_primes(notDivisible);
    }
    max_computed = n;
    return cache[n];
}
