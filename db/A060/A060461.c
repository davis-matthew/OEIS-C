/*
 * A060461  Numbers k such that 6*k-1 and 6*k+1 are twin composites.
 * https://oeis.org/A060461
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* IEEE: A060461 – Numbers k such that both 6*k-1 and 6*k+1 are composite.
   The function a(n) is computed by successive search: k is tested from 1
   upwards and stored when both 6k-1 and 6k+1 are non‑prime.  Cache is used
   so that the function works correctly for arbitrary ordering of calls. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- primality test (trial division) ---------- */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2)          return false;
    if (n % 2 == 0)     return n == 2;
    for (uint64_t d = 3; d * d <= n; d += 2)
        if (n % d == 0) return false;
    return true;
}

/* ---------- cache for results ---------- */
static seq_t *cache = NULL;   /* a(1) is stored at cache[0] */
static size_t cache_len = 0;  /* number of elements in cache      */
static size_t cache_cap = 0;  /* allocated capacity                */
static idx_t next_k = 1;      /* next k to test                    */

/* ensure capacity of cache array */
static void cache_grow(void)
{
    if (cache_len < cache_cap)
        return;
    cache_cap = cache_cap ? cache_cap * 2 : 16;
    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
}

seq_t a(idx_t n)
{
    if (n == 0)           /* out of spec; return 0 safely */
        return 0;

    /* build more terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        uint64_t k = (uint64_t)next_k++;
        uint64_t p = 6 * k - 1;
        uint64_t q = 6 * k + 1;
        if (!is_prime_uint64(p) && !is_prime_uint64(q)) {
            cache_grow();
            cache[cache_len++] = (seq_t)k;
        }
    }
    return cache[n - 1];
}
