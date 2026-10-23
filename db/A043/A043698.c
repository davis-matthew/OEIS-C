/*
 * A043698  Numbers whose base-9 representation has an even number of runs.
 * https://oeis.org/A043698
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers whose base‑9 representation has an even number of runs
 *            (maximal blocks of equal digits). The function scans integers in
 *            ascending order, keeping a cache of those that satisfy the
 *            parity condition. It returns the nth such integer, with a(1)=9
 * Range:     u8 n <= 98, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Return true iff x in base‑9 has an even number of runs. */
static bool even_runs(idx_t x)
{
    unsigned runs = 0U;
    unsigned prev = 9U;          /* sentinel outside 0‑8 range */
    while (x != 0U)
    {
        unsigned d = (unsigned)(x % 9U);
        if (d != prev)
        {
            ++runs;
            prev = d;
        }
        x /= 9U;
    }
    return (runs & 1U) == 0U;
}

static idx_t *cache   = NULL;  /* array of found integers (idx_t)  */
static size_t  cap    = 0;     /* current capacity of cache        */
static size_t  cnt    = 0;     /* number of cached numbers         */
static idx_t   last   = 0;     /* last integer examined            */

seq_t a(idx_t n)
{
    if (n == 0U) return (seq_t)0U;   /* offset 1; not used but harmless */

    /* initialise cache if needed */
    if (cap == 0U)
    {
        cap   = 1024U;
        cache = (idx_t *)malloc(cap * sizeof(idx_t));
    }

    /* extend cache until we have n numbers */
    while (cnt < n)
    {
        ++last;
        if (even_runs(last))
        {
            if (cnt >= cap)
            {
                cap <<= 1U;                     /* double capacity       */
                cache = (idx_t *)realloc(cache, cap * sizeof(idx_t));
            }
            cache[cnt++] = last;
        }
    }
    return (seq_t)cache[n - 1U];
}
