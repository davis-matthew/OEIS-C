/*
 * A039596  Numbers that are simultaneously triangular and square pyramidal.
 * https://oeis.org/A039596
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, nonn, full
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers that are simultaneously triangular and square pyramidal.
   The nth term is the nth smallest value N such that
       N = r(r+1)(2r+1)/6            (square‑pyramidal)
   and          N = s(s+1)/2          (triangular)
   Equivalently, 1 + 8*N must be an odd perfect square.  */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/floor_sqrt.h"

static seq_t *cache = NULL;      /* array of found terms                 */
static size_t cacheSize = 0;     /* # of terms stored                   */
static size_t cacheCap  = 0;     /* capacity of cache                   */
static idx_t  lastR = 0;         /* last r examined                     */
static seq_t currentP = 0;       /* P_{lastR}, the current square‑pyramidal value */

/* Expand the cache if needed. */
static void grow_cache(size_t needed)
{
    if (cacheCap >= needed) return;
    size_t newCap = cacheCap ? cacheCap * 2 : 64;
    while (newCap < needed) newCap <<= 1;
    cache = (seq_t *)realloc(cache, newCap * sizeof(seq_t));
    cacheCap = newCap;
}

/* Generate solutions until we have at least n terms cached. */
static void ensure_up_to(idx_t n)
{
    while (cacheSize < n)
    {
        /* Test currentP for the Pell condition. */
        if (sizeof(seq_t) == 8)  /* 64‑bit build: protect overflow. */
        {
            unsigned __int128 d128 = (unsigned __int128)currentP * 8 + 1;
            if (d128 > (unsigned __int128)UINT64_MAX) break;
            seq_t d = (seq_t)d128;
            seq_t m = floor_sqrt(d);
            if (m * m == d && (m & 1) == 1)
            {
                grow_cache(cacheSize + 1);
                cache[cacheSize++] = currentP;
            }
        }
        else  /* arbitrary–precision build. */
        {
            seq_t d = (seq_t)8 * currentP + (seq_t)1;
            seq_t m = floor_sqrt(d);
            if (m * m == d && (m & 1) == 1)
            {
                grow_cache(cacheSize + 1);
                cache[cacheSize++] = currentP;
            }
        }

        /* Move to the next r, updating the square‑pyramidal sum. */
        ++lastR;
        if (sizeof(seq_t) == 8)  /* 64‑bit build: avoid overflow. */
        {
            unsigned __int128 next128 =
                (unsigned __int128)currentP + (unsigned __int128)lastR * lastR;
            if (next128 > (unsigned __int128)UINT64_MAX) break;
            currentP = (seq_t)next128;
        }
        else  /* arbitrary–precision build. */
        {
            currentP += (seq_t)lastR * lastR;
        }
    }
}

/* External function required by the framework. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                      /* safety, a(0) undefined  */
    ensure_up_to(n);
    return cache[n - 1];
}
