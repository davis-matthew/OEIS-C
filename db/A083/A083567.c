/*
 * A083567  Let B(k) be the number of binary digits in k equal to 1. This is
 *          the sequence of positive integers k such that 2B(k)=B(k^2).
 * https://oeis.org/A083567
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 6176, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find integers k such that popcount(k^2) == 2 * popcount(k). */
/* The function builds the sequence incrementally, caching each hit so that
   subsequent calls are answered in O(1) time (apart from the time to
   generate missing terms). */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Cached values of the sequence */
static seq_t *seq_cache = NULL;   /* array of seq_t values */
static size_t seq_len   = 0;       /* number of cached terms */
static size_t seq_cap   = 0;       /* current capacity of seq_cache */
static idx_t   next_k    = 1;      /* next integer k to test */

/* Ensure the cache has room for 'need' terms. */
static void ensure_cache(size_t need)
{
    if (need <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 8;
    while (new_cap < need) new_cap <<= 1;
    seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* Count set bits in a 64‑bit integer. */
static inline unsigned int popcnt64(uint64_t v)
{
    return (unsigned int)__builtin_popcountll(v);
}

/* Test for a given k whether 2*popcount(k) == popcount(k^2). */
static inline bool good_k(uint64_t k)
{
    unsigned int c1 = popcnt64(k);
    __uint128_t sq = (__uint128_t)k * k;
    unsigned int c2 = popcnt64((uint64_t)sq) + popcnt64((uint64_t)(sq >> 64));
    return 2 * c1 == c2;
}

/* Main sequence routine. */
seq_t a(idx_t n)
{
    /* Extend cache until it contains at least n terms. */
    while (seq_len < n)
    {
        if (good_k((uint64_t)next_k))
        {
            ensure_cache(seq_len + 1);
            seq_cache[seq_len++] = (seq_t)next_k;   /* cast to seq_t */
        }
        ++next_k;
    }
    return seq_cache[n - 1];
}
