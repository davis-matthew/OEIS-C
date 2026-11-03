/*
 * A071336  Number of vertices of Goldberg-Casper-Klug pseudo-icosahedra.
 * https://oeis.org/A071336
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*A003136(n+1) + 2
 * Range:     u8 n <= 11, u16 n <= 1200, u32 n <= 1200, u64 n <= 1200,
 *            u128 n <= 1200, big any n
 * Validated: standard, pass, 66 terms through n = 1072 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static bool is_loeschian_uint64(uint64_t m)
{
    if (m == 0) return true;
    uint64_t n = m;
    for (uint64_t p = 2; p * p <= n; ++p)
    {
        if (n % p == 0)
        {
            int exp = 0;
            while (n % p == 0)
            {
                n /= p;
                ++exp;
            }
            if ((p % 3) == 2 && (exp & 1)) return false;
        }
    }
    return !(n > 1 && (n % 3) == 2);
}

seq_t a(idx_t n)
{
    /* cache first Loeschian numbers for successive calls */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t cache_used = 0;
    static uint64_t next_val = 0;
    static bool initialized = false;

    if (!initialized)
    {
        cache_cap = 64;
        cache = mem_new(seq_t, cache_cap);
        cache[0] = (seq_t)0;   /* A003136(1) = 0 */
        cache_used = 1;
        next_val = 0;
        initialized = true;
    }

    idx_t target = n + 1;            /* we need the (n+1)-th Loeschian number */
    while (cache_used < target)
    {
        /* find next Loeschian number */
        do { ++next_val; } while (!is_loeschian_uint64(next_val));

        /* grow cache if necessary */
        if (cache_used >= cache_cap)
        {
            cache_cap <<= 1;
            cache = mem_resize(seq_t, cache, cache_cap);
        }

        cache[cache_used++] = (seq_t)next_val;
    }

    seq_t L = cache[target - 1];     /* L = A003136(n+1) */
    seq_t result = (seq_t)10 * L + (seq_t)2;
    return result;
}
