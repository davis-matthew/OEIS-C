/*
 * A092609  Product of first n primes that end in 1.
 * https://oeis.org/A092609
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = product of the first n primes that end in 1 (1‑mod‑10)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 18,
 *            big any n
 * Helpers:   seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/seq_alloc_new_array.h"

/* Static state */
static size_t max_limit   = 0;          /* highest limit used in a sieve */
static size_t primes_cnt  = 0;          /* number of primes found */
static uint64_t *primes   = NULL;       /* all primes up to max_limit */
static size_t primes1_cnt = 0;          /* number of primes ending in 1 */
static uint64_t *primes1  = NULL;       /* primes ending in 1 */
static seq_t *prod_arr    = NULL;       /* cumulative products: prod_arr[i] = product of first i primes1 */
static size_t prod_len    = 0;          /* prod_arr size ( = primes1_cnt + 1 ) */

/* Rebuild the prime tables up to a new limit.  It discards previous data. */
static void rebuild_primes(size_t limit)
{
    /* free previous allocations */
    free(primes);
    free(primes1);
    free(prod_arr);
    primes  = NULL;
    primes1 = NULL;
    prod_arr = NULL;
    primes_cnt = primes1_cnt = 0;
    max_limit = limit;

    /* Sieve of Eratosthenes */
    uint8_t *isprime = (uint8_t*)malloc(limit + 1);
    for (size_t i = 0; i <= limit; ++i) isprime[i] = 1;
    isprime[0] = isprime[1] = 0;
    for (uint64_t p = 2; p * p <= limit; ++p) {
        if (isprime[p]) {
            for (uint64_t m = p * p; m <= limit; m += p)
                isprime[m] = 0;
        }
    }

    /* Allocate dynamic arrays */
    size_t primes_cap = 16, p1_cap = 16;
    primes  = (uint64_t*)malloc(primes_cap  * sizeof(uint64_t));
    primes1 = (uint64_t*)malloc(p1_cap * sizeof(uint64_t));

    /* Enumerate primes */
    for (uint64_t num = 2; num <= limit; ++num) {
        if (isprime[num]) {
            if (primes_cnt >= primes_cap) {
                primes_cap <<= 1;
                primes = (uint64_t*)realloc(primes, primes_cap * sizeof(uint64_t));
            }
            primes[primes_cnt++] = num;

            if ((num % 10U) == 1U) {
                if (primes1_cnt >= p1_cap) {
                    p1_cap <<= 1;
                    primes1 = (uint64_t*)realloc(primes1, p1_cap * sizeof(uint64_t));
                }
                primes1[primes1_cnt++] = num;
            }
        }
    }

    /* Compute cumulative products */
    prod_arr = seq_alloc_new_array(primes1_cnt + 1);
    prod_arr[0] = (seq_t)1;
    for (size_t i = 0; i < primes1_cnt; ++i)
        prod_arr[i + 1] = prod_arr[i] * (seq_t)primes1[i];
    prod_len = primes1_cnt + 1;

    free(isprime);
}

/* Ensure that at least 'n' primes that end in 1 are available. */
static void ensure_primes(idx_t n)
{
    if (primes1_cnt >= (size_t)n) return;

    size_t limit = max_limit ? max_limit : 200;
    while (primes1_cnt < (size_t)n) {
        limit <<= 1;               /* double the limit each iteration */
        rebuild_primes(limit);
    }
}

/* The sequence function requested by the OEIS entry. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset 1 – so a(0) is unused */
    ensure_primes(n);
    return prod_arr[(size_t)n];
}
