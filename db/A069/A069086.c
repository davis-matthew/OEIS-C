/*
 * A069086  Treated as strings, n begins with Floor(sqrt(n)).
 * https://oeis.org/A069086
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   digits_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: a(n) = the n-th positive integer whose decimal representation
 * begins with floor(sqrt(n)).  The sequence is built by testing each integer
 * in increasing order and caching the ones that satisfy the property. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digits_u64.h"

/* compute floor(sqrt(x)) for 64‑bit unsigned integers */
static uint64_t floor_isqrt_u64(uint64_t n)
{
    if (n == 0) return 0;
    uint64_t low = 0, high = 1ULL << 32;          /* sqrt(2^64‑1) < 2^32 */
    while (low < high)
    {
        uint64_t mid = (low + high) >> 1;
        if (mid > 0 && mid > n / mid) high = mid;
        else low = mid + 1;
    }
    return low - 1;
}

/* test whether an integer satisfies the condition */
static bool has_property_u64(uint64_t val)
{
    uint64_t m    = floor_isqrt_u64(val);          /* floor(sqrt(val)) */
    int     dv    = digits_u64(val);
    int     dm    = digits_u64(m);
    int     exp   = dv - dm;                       /* shift to get prefix */

    uint64_t pow10 = 1;
    for (int i = 0; i < exp; ++i) pow10 *= 10ULL;

    uint64_t prefix = val / pow10;
    return prefix == m;
}

/* cache storage for computed terms */
static seq_t *cache      = NULL;
static size_t cache_cap  = 0;
static size_t cache_size = 0;
static uint64_t next_candidate = 1;   /* next integer to test */

/* Public function required by the exercise */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset is 1, but guard for safety */

    /* generate terms until we have n of them */
    while (cache_size < (size_t)n)
    {
        if (has_property_u64(next_candidate))
        {
            /* allocate or grow cache as needed */
            if (cache_size == cache_cap)
            {
                cache_cap = (cache_cap == 0) ? 64 : cache_cap << 1;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_size++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
    return cache[n - 1];
}
