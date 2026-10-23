/*
 * A105146  Numbers k such that the string k100001 is prime.
 * https://oeis.org/A105146
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   ensure_capacity, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for numbers k such that the decimal concatenation of k and 100001
   is prime:  k*10^6 + 100001.  The function keeps a memoised list of all
   k found so far, enabling arbitrary order of n. */
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/ensure_capacity.h"

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
        if (is_prime(N))
        {
            ensure_cache_cap(cache_size + 1);
            cache[cache_size] = last_k_checked;   /* implicit conversion to seq_t */
            ++cache_size;
        }
    }

    return cache[n - 1];
}
