/*
 * A105146  Numbers k such that the string k100001 is prime.
 * https://oeis.org/A105146
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 75 terms through n = 915 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for numbers k such that the decimal concatenation of k and 100001
   is prime:  k*10^6 + 100001.  The function keeps a memoised list of all
   k found so far, enabling arbitrary order of n. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

/* Grow an array of seq_t elements to at least min_cap elements. */
static void ensure_capacity(size_t min_cap, seq_t **arr, size_t *cap)
{
    if (min_cap <= *cap) return;
    size_t new_cap = (*cap) ? (*cap) * 2 : 1024;
    if (new_cap < min_cap) new_cap = min_cap;
    *arr = mem_resize(seq_t, *arr, new_cap);
    *cap = new_cap;
}

static seq_t *cache      = NULL;   /* cached k values */
static idx_t  cache_size = 0;      /* number of stored terms */
static size_t cache_cap  = 0;      /* allocated capacity */
static uint64_t last_k_checked = 0;/* largest k examined */

static void ensure_cache_cap(idx_t min_cap)
{
    if (cache_cap >= (size_t)min_cap) return;
    ensure_capacity(min_cap, &cache, &cache_cap);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset 1, undefined for 0 */

    while (cache_size < n)
    {
        last_k_checked++;                          /* next k to test */
        uint64_t N = last_k_checked * 1000000ULL + 100001ULL;
        if (is_prime_u64(N))
        {
            ensure_cache_cap(cache_size + 1);
            cache[cache_size] = last_k_checked;   /* implicit conversion to seq_t */
            ++cache_size;
        }
    }

    return cache[n - 1];
}
