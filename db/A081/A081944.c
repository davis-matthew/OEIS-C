/*
 * A081944  Numbers k such that both k*(k+1) - 1 and k*(k-1) - 1 are primes.
 * https://oeis.org/A081944
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 1800, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrime64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A081944:
 *  a(n) = the n-th integer k such that both k*(k+1)-1 and k*(k-1)-1 are prime.
 *  The function searches for such k sequentially and caches found values.
 *  It uses the deterministic 64‑bit primality test isPrime64() from the
 *  utilities. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/isPrime64.h"   /* provides bool isPrime64(uint64_t) */

/* static cache of found ks (stored as uint64_t for ease of storage) */
static uint64_t *k_cache      = NULL;   /* dynamic array of ks   */
static idx_t     k_cache_len  = 0;      /* number of elements   */
static idx_t     k_cache_cap  = 0;      /* allocated capacity   */
static uint64_t  next_k       = 3;      /* next k to test       */

/* ensure that k_cache contains at least n elements */
static void ensure_cache(idx_t n)
{
    while (k_cache_len < n) {
        /* grow storage when needed */
        if (k_cache_len == k_cache_cap) {
            k_cache_cap = k_cache_cap ? k_cache_cap * 2u : 4u;
            uint64_t *tmp = (uint64_t *)realloc(k_cache, k_cache_cap * sizeof(uint64_t));
            if (!tmp) abort();          /* allocation failed */
            k_cache = tmp;
        }

        uint64_t k = next_k++;
        /* compute k*(k-1)-1 and k*(k+1)-1 using 128‑bit to avoid overflow */
        unsigned __int128 a = (unsigned __int128)k * (k - 1);
        unsigned __int128 b = (unsigned __int128)k * (k + 1);
        uint64_t val1 = (uint64_t)(a - 1);
        uint64_t val2 = (uint64_t)(b - 1);

        if (isPrime64(val1) && isPrime64(val2)) {
            k_cache[k_cache_len++] = k;
        }
    }
}

/* O(n) sequence generator with memoization */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; undefined for n==0 */
    if (!k_cache) {                /* initialise on first call */
        k_cache_cap = 0;
        k_cache_len = 0;
        k_cache     = NULL;
        next_k      = 3;
    }
    ensure_cache(n);
    return (seq_t)k_cache[n - 1];
}
