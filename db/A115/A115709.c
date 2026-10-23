/*
 * A115709  Pentagonal numbers (A000326) whose digit reversal is a semiprime
 *          (A001358).
 * https://oeis.org/A115709
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 56, u32 n <= 137, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   _smallest_prime_factor, is_prime_big, rev, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A115709: pentagonal numbers P(k)=k*(3k-1)/2 whose digit reversal is a semiprime. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/_smallest_prime_factor.h"
#include "../utils/is_prime_big.h"
#include "../utils/rev.h"
#include "../utils/seq_realloc.h"

static seq_t *cache = NULL;          /* cached valid pentagonal numbers */
static size_t cache_len = 0;         /* number of cached entries          */
static size_t cache_cap = 0;         /* current allocated capacity        */
static idx_t  last_k   = 0;          /* last k processed in the sieve    */

/* grow the cache to at least new_cap entries */
static void grow_cache(size_t new_cap)
{
    if (new_cap <= cache_cap) return;
    seq_t *p = seq_realloc(cache, cache_cap, new_cap);
    if (!p) abort();                /* out of memory */
    cache   = p;
    cache_cap = new_cap;
}

/* check whether x is a semiprime (product of exactly two primes) */
static bool is_semiprime_uint64(uint64_t x)
{
    if (x < 4) return false;               /* 4 = 2*2 is the smallest semiprime */
    uint64_t spf = _smallest_prime_factor(x);
    if (spf <= 1) return false;
    uint64_t q = x / spf;
    return is_prime_big((seq_t)q);
}

/* the required sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset is 1 – safety return */

    /* generate terms until we have at least n cached */
    while (cache_len < (size_t)n)
    {
        last_k++;                                   /* next k */
        seq_t k = (seq_t)last_k;
        seq_t pent = k * (3 * k - 1) / 2;            /* pentagonal number */

        uint64_t rev_val = (uint64_t)rev((idx_t)pent);
        if (is_semiprime_uint64(rev_val))
        {
            if (cache_len == cache_cap) grow_cache(cache_cap ? cache_cap * 2 : 16);
            cache[cache_len++] = pent;
        }
    }

    return cache[(size_t)(n - 1)];                 /* 1‑based indexing */
}
