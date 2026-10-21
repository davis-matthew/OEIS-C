/*
 * A056745  Numbers k such that k | 6^k + 5^k + 4^k + 3^k + 2^k + 1^k.
 * https://oeis.org/A056745
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth integer k such that k | (1^k + 2^k + 3^k + 4^k +
 *            5^k + 6^k)
 * Range:     u8 n <= 13, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This implementation keeps a static cache of the terms that have been
    discovered so far.  For each new term it tests successive positive
    integers k and uses binary exponentiation (with 128‑bit intermediate
    products) to compute b^k mod k for b = 1..6.  Whenever the sum of the
    six residues is congruent to 0 modulo k, k is added to the cache. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/*  Helper to compute b^e  (mod m) using binary exponentiation.
    All arithmetic is performed with 128‑bit intermediates so that the
    product of two 64‑bit numbers never overflows.  The result is
    guaranteed to fit in an unsigned 64‑bit integer (the modulus). */
static uint64_t mod_pow_uint64(uint64_t b, uint64_t e, uint64_t m)
{
    uint64_t result = 1 % m;
    b %= m;
    while (e)
    {
        if (e & 1ULL)
            result = (unsigned __int128)result * b % m;
        b = (unsigned __int128)b * b % m;
        e >>= 1ULL;
    }
    return result;
}

/*  Cache of found terms. */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;   /* number of stored terms */
static size_t cache_cap  = 0;   /* allocated capacity of cache */
static idx_t  tested_k   = 1;   /* next k to test */

/*  Ensure that the cache contains at least 'need' terms. */
static void extend_cache(size_t need)
{
    if (cache_len >= need) return;

    /* Allocate or grow the cache array if necessary. */
    if (cache_cap == 0)
    {
        cache_cap = 8;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }
    while (cache_cap < need)
        cache_cap <<= 1;
    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));

    /* Test successive k values until enough terms are found. */
    while (cache_len < need)
    {
        uint64_t k = (uint64_t)tested_k;
        uint64_t sum = 0;

        for (uint64_t base = 1; base <= 6; ++base)
        {
            uint64_t term = mod_pow_uint64(base, tested_k, k);
            sum += term;
            if (sum >= k) sum -= k;   /* keep sum modulo k */
        }

        if (sum == 0)
            cache[cache_len++] = (seq_t)tested_k;

        ++tested_k;
    }
}

/*  Required external function.  The build supplies the types
    idx_t and seq_t.  No other global state is required beyond the
    static cache defined above. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined, harmless for n==0 */
    extend_cache((size_t)n);
    return cache[n - 1];
}
