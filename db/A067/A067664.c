/*
 * A067664  Numbers n such that n^2 + 1 and n^2 + n + 1 are primes.
 * https://oeis.org/A067664
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is the nth integer m such that m^2+1 and m^2+m+1 are primes
 * Range:     u8 n <= 4, u16 n <= 14, u32 n <= 553, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   isPrimeGeneric, seq_alloc_new_array, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/isPrimeGeneric.h"
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_realloc.h"

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

static void extend_cache(idx_t target)
{
    while ((idx_t)cache_len < target)
    {
        /* Start searching from the next integer */
        seq_t last = (cache_len == 0) ? (seq_t)0 : cache[cache_len - 1];
        seq_t m = last + (seq_t)1;

        for (;;)
        {
            seq_t mm  = m * m;
            seq_t a   = mm + (seq_t)1;
            seq_t b   = mm + m + (seq_t)1;

            if (isPrimeGeneric(a) && isPrimeGeneric(b))
            {
                /* Ensure capacity for the new element */
                if (cache_cap == cache_len)
                {
                    size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                    seq_t *newp    = seq_realloc(cache, cache_cap, new_cap);
                    cache          = newp;
                    cache_cap      = new_cap;
                }
                cache[cache_len++] = m;
                break; /* candidate found, move to next n */
            }
            m = m + (seq_t)1;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset starts at 1 – behaviour for 0 is undefined */
    extend_cache(n);
    return cache[n - 1];
}
