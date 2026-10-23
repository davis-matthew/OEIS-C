/*
 * A110411  Smallest prime beginning with the n-th composite number.
 * https://oeis.org/A110411
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: the smallest prime that begins with the n‑th composite number. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"     /* bool is_prime(uint64_t n); */

/* static cache for composite numbers */
static uint64_t *composite_cache = NULL;
static size_t   composite_cap   = 0;
static size_t   composite_len   = 0;
static uint64_t next_candidate   = 4;

/* Extend the cache until we have at least n composites. */
static void ensure_composites(idx_t n)
{
    while (composite_len < n)
    {
        /* find the next composite number */
        uint64_t x = next_candidate;
        while (is_prime(x)) ++x;

        /* grow the array if needed */
        if (composite_len == composite_cap)
        {
            composite_cap = composite_cap ? composite_cap * 2 : 8;
            composite_cache = (uint64_t *)realloc(composite_cache,
                                                  composite_cap * sizeof(uint64_t));
        }

        composite_cache[composite_len++] = x;
        next_candidate = x + 1;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* OEIS offset: a(1) is the first defined term.  For n==0 nothing is defined. */
    if (n == 0) return (seq_t)0;

    ensure_composites(n);
    uint64_t composite = composite_cache[n - 1];

    uint64_t pow10 = 1;                 /* 10^k */
    for (;;)
    {
        uint64_t start = composite * pow10;
        uint64_t end   = start + pow10 - 1;

        /* Scan all numbers with this prefix */
        for (uint64_t x = start; x <= end; ++x)
        {
            if (is_prime(x)) return (seq_t)x;
        }

        /* Prepare next higher k, guard against overflow */
        if (pow10 > UINT64_MAX / 10) break;   /* should never happen for required ranges */
        pow10 *= 10;
    }
    /* Theoretically unreachable: there is always a prime with any given prefix. */
    return (seq_t)0;
}
