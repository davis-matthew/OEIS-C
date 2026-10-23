/*
 * A125822  Numbers k for which 8*k + 1 and 8*k + 3 are twin primes.
 * https://oeis.org/A125822
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 1182, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by iterating m = 0, 1, 2, … and selecting k = 3*m+2 when
   8*k+1 and 8*k+3 are both prime.  The cache stores the k values
   lazily, so any order of calls to a(n) is supported. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static seq_t *ks_cache = NULL;     /* cache of found k values   */
static size_t ks_cache_cap = 0;    /* current capacity           */
static size_t ks_cache_sz  = 0;    /* number of values stored   */
static uint64_t ks_cur_m = 0;      /* next m to test            */

static void ks_grow(size_t min_cap)
{
    if (min_cap <= ks_cache_cap)
        return;
    size_t new_cap = ks_cache_cap ? ks_cache_cap << 1 : 16;
    while (new_cap < min_cap)
        new_cap <<= 1;
    seq_t *new_ptr = (seq_t *)realloc(ks_cache, new_cap * sizeof(seq_t));
    if (!new_ptr)
        return;   /* out of memory – future accesses will fail */
    ks_cache = new_ptr;
    ks_cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined for offset 1, but safe          */
    /* Ensure the cache holds at least n values */
    while (ks_cache_sz < n)
    {
        /* k = 3*m + 2  -->  8*k+1 = 24*m+17,   8*k+3 = 24*m+19 */
        uint64_t p  = 24ULL * ks_cur_m + 17ULL;
        uint64_t p2 = p + 2ULL;
        if (is_prime(p) && is_prime(p2))
        {
            seq_t k = (seq_t)(3ULL * ks_cur_m + 2ULL);
            ks_grow(ks_cache_sz + 1);
            ks_cache[ks_cache_sz++] = k;
        }
        ks_cur_m++;
    }
    return ks_cache[n - 1];   /* 1‑based indexing as in OEIS          */
}
