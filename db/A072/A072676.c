/*
 * A072676  Numbers k for which the prime circle problem has a solution
 *          composed of disjoint subsets: the arrangement of numbers 1 through
 *          2k around a circle is such that the sum of each pair of adjacent
 *          numbers is prime, the odd numbers are in order and the even
 *          numbers are in groups of decreasing sequences.
 * https://oeis.org/A072676
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nice, nonn
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS A072676.
   A number k is in the sequence if
   (a) 4k‑1 and 2k+1 are prime,  or
   (b) there exists 0<i<k such that
       2k+2i‑1, 2k+2i+1 and 2i+1 are prime.
   This function returns the nth term a(n) (nth k) with offset 1
   by generating the sequence on demand and caching the results. */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Public entry point used by the framework. */
extern seq_t a(idx_t n);

/* --------------------------------------------------------------------- */
/* Constants and static state */
/* --------------------------------------------------------------------- */
#pragma once

/* Upper bound for prime sieve.  The value 30,000,010 comfortably covers
   all k needed for n up to at least 100,000, which is well beyond the
   typical limits of this problem. */
#define SIEVE_UPPER 30000010ULL

static unsigned char *prime = NULL;      /* 1 = prime, 0 = composite */
static size_t prime_limit = 0;           /* number of entries in prime[] */

static seq_t *cache = NULL;              /* stored sequence values */
static size_t cache_cap = 0;             /* current capacity */
static size_t cache_len = 0;             /* number of stored terms */
static idx_t   last_k   = 0;             /* largest k processed so far */

/* --------------------------------------------------------------------- */
/* Prime sieve                                                */
/* --------------------------------------------------------------------- */
static void init_sieve(void)
{
    if (prime) return;   /* already initialized */

    /* Allocate singed array of bits */
    prime = (unsigned char*)calloc(SIEVE_UPPER, sizeof(unsigned char));
    if (!prime) abort();

    prime_limit = SIEVE_UPPER;

    /* Mark all as prime initially, except 0 and 1 */
    for (size_t i = 2; i < prime_limit; ++i) prime[i] = 1;
    if (prime_limit > 0) prime[0] = 0;
    if (prime_limit > 1) prime[1] = 0;

    /* Eratosthenes sieve */
    /* Find integer sqrt(prime_limit) */
    size_t sqrt_lim = 2;
    for ( ; (sqrt_lim + 1) * (sqrt_lim + 1) <= prime_limit; ++sqrt_lim) ;
    for (size_t p = 2; p <= sqrt_lim; ++p)
        if (prime[p]) {
            for (size_t m = p * p; m < prime_limit; m += p)
                prime[m] = 0;
        }
}

/* --------------------------------------------------------------------- */
/* Helper to add a value to the cache                                 */
/* --------------------------------------------------------------------- */
static void cache_add(seq_t val)
{
    if (cache_len >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* --------------------------------------------------------------------- */
/* Generate terms until cache_len >= n                                  */
/* --------------------------------------------------------------------- */
static void generate_up_to(idx_t n)
{
    while (cache_len < (size_t)n) {
        ++last_k;                 /* candidate k */

        bool satisfies = false;

        /* Condition (a): 4k-1 and 2k+1 prime */
        idx_t a1 = 4 * last_k - 1;
        idx_t a2 = 2 * last_k + 1;
        if (a1 < prime_limit && a2 < prime_limit &&
            prime[(size_t)a1] && prime[(size_t)a2]) {
            satisfies = true;
        } else {
            /* Condition (b): search for i with 0<i<k */
            for (idx_t i = 1; i < last_k; ++i) {
                idx_t p = 2 * i + 1;              /* 2i+1 */
                if (p >= prime_limit || !prime[(size_t)p]) continue;
                idx_t b1 = 2 * last_k + p - 2;    /* 2k+2i-1 */
                idx_t b2 = 2 * last_k + p;        /* 2k+2i+1 */
                if (b1 >= prime_limit || b2 >= prime_limit) continue;
                if (prime[(size_t)b1] && prime[(size_t)b2]) {
                    satisfies = true;
                    break;
                }
            }
        }

        if (satisfies) {
            cache_add((seq_t)last_k);
        }
    }
}

/* --------------------------------------------------------------------- */
/* Public function                                                  */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* undefined for n=0, safe default */

    init_sieve();
    generate_up_to(n);

    return cache[(size_t)(n - 1)];
}
