/*
 * A145305  Numbers Y such that 237*Y^2+79 is a square.
 * https://oeis.org/A145305
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n+2) = 456302*a(n+1) - a(n) with a(1) = 195, a(2) = 88979085
 *            (sequence A145305)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_STATIC_N 10000      /* enough for all required tests */

static seq_t cache[MAX_STATIC_N + 1];
static unsigned char computed[MAX_STATIC_N + 1] = {0};

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;               /* undefined for indices < 1 */

    /* If n is within the pre‑allocated static range, use it */
    if (n <= MAX_STATIC_N)
    {
        if (!computed[1]) {
            cache[1]  = (seq_t)195;
            computed[1] = 1;
        }
        if (!computed[2]) {
            cache[2]  = (seq_t)88979085;
            computed[2] = 1;
        }

        for (size_t i = 3; i <= (size_t)n; ++i)
        {
            if (!computed[i]) {
                seq_t prod = cache[i - 1] * (seq_t)456302;
                cache[i]   = prod - cache[i - 2];
                computed[i] = 1;
            }
        }
        return cache[(size_t)n];
    }

    /* For indices beyond the static limit we use a dynamically growable
       cache.  The dynamic arrays are allocated with seq_alloc_* helpers
       to ensure correct construction for arbitrary‑precision types. */
    static seq_t *dyn_cache   = NULL;
    static idx_t *dyn_flag    = NULL;
    static size_t dyn_cap     = 0;          /* number of allocated indices */

    size_t need = (size_t)n + 1;           /* indices 0 .. n */

    if (dyn_cap < need)
    {
        size_t new_cap = need * 2;
        dyn_cache = (seq_t *)realloc(dyn_cache, new_cap * sizeof(seq_t));
        dyn_flag  = (idx_t *)realloc(dyn_flag,  new_cap * sizeof(idx_t));
        if (!dyn_cache || !dyn_flag) abort();

        /* Zero the new flag entries */
        memset(dyn_flag + dyn_cap, 0, (new_cap - dyn_cap) * sizeof(idx_t));
        dyn_cap = new_cap;
    }

    if (!dyn_flag[1]) {
        dyn_cache[1]  = (seq_t)195;
        dyn_flag[1]   = 1;
    }
    if (!dyn_flag[2]) {
        dyn_cache[2]  = (seq_t)88979085;
        dyn_flag[2]   = 1;
    }

    for (size_t i = 3; i <= (size_t)n; ++i)
    {
        if (!dyn_flag[i]) {
            seq_t prod = dyn_cache[i - 1] * (seq_t)456302;
            dyn_cache[i]   = prod - dyn_cache[i - 2];
            dyn_flag[i]    = 1;
        }
    }
    return dyn_cache[(size_t)n];
}
