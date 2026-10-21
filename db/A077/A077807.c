/*
 * A077807  Number of distinct prime factors of numbers containing in their
 *          decimal representation only the digits 0 and 1.
 * https://oeis.org/A077807
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) = number of distinct prime factors of the
   decimal number obtained by writing n in binary and interpreting
   the digits as decimal 0/1.  i.e.  a(n) = A001221(A007088(n)). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

/* static storage for memoisation */
static seq_t *cache      = NULL;   /* cache[n-1] = a(n) */
static size_t cache_cap  = 0;      /* allocated capacity */
static idx_t  cache_last = 0;      /* number of computed terms */

/* prime list used for factorisation */
static uint64_t *prime_list = NULL;   /* primes up to 4,000,000 */
static size_t    prime_count = 0;

/* initialises prime list once */
static void init_prime_list(void)
{
    if (prime_list != NULL)
        return;
    /* 4,000,000 > sqrt(10^14) ≈ 3.3 × 10^6, enough for all numbers
       that appear for n up to 10 000. */
    build_primes_upto(4000000, &prime_list, &prime_count);
}

/* converts the binary representation of n into the corresponding
   decimal number consisting only of digits 0 and 1.  The result
   fits in uint64_t for all n relevant to this problem. */
static uint64_t binary_to_decimal(idx_t n)
{
    uint64_t v = 0;
    idx_t msb = 1;
    /* find most‑significant bit of n */
    while (msb <= n)
        msb <<= 1;
    msb >>= 1;
    /* construct the decimal number */
    while (msb) {
        v = v * 10 + ((n & msb) ? 1 : 0);
        msb >>= 1;
    }
    return v;
}

/* counts distinct prime factors of a 64‑bit integer using the
   previously built prime list. */
static uint64_t num_distinct_primes(uint64_t m)
{
    uint64_t cnt = 0;
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > m)
            break;
        if (m % p == 0) {
            ++cnt;
            while (m % p == 0)
                m /= p;
        }
    }
    if (m > 1)
        ++cnt;
    return cnt;
}

/* grows the cache array to hold at least 'needed' elements */
static void ensure_cache_size(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) {
        /* allocation failure – terminate with diagnostic */
        abort();
    }
    cache     = new_cache;
    cache_cap = new_cap;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* behaviour below the offset is irrelevant */

    init_prime_list();

    /* compute terms up to n if not already done */
    if (cache_last < n) {
        ensure_cache_size(n);
        for (idx_t i = cache_last + 1; i <= n; ++i) {
            uint64_t dec      = binary_to_decimal(i);
            uint64_t distinct = num_distinct_primes(dec);
            cache[cache_last++] = (seq_t)distinct;
        }
    }
    return cache[n - 1];
}
