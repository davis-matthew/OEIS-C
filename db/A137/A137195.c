/*
 * A137195  Even-indexed coefficients related to Kirchhoff index of hexagonal
 *          (benzene) chains.
 * https://oeis.org/A137195
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 12*a(n-2)-38*a(n-4)+12*a(n-6)-a(n-8); Initial values:
 *            a(0)=1, a(1)=6, a(2)=19, a(3)=64, a(4)=185, a(5)=542, a(6)=1511,
 *            a(7)=4144
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 21, u64 n <= 45, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;

static void ensure_cache(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (needed <= cache_cap)
        return;

    seq_t *newptr = (seq_t *)realloc(cache, needed * sizeof(seq_t));
    if (!newptr)
        exit(1); /* memory allocation failed */

    /* Initialize new elements */
    for (size_t i = cache_cap; i < needed; ++i)
    {
        if (i == 0)          newptr[i] = (seq_t)1;
        else if (i == 1)     newptr[i] = (seq_t)6;
        else if (i == 2)     newptr[i] = (seq_t)19;
        else if (i == 3)     newptr[i] = (seq_t)64;
        else if (i == 4)     newptr[i] = (seq_t)185;
        else if (i == 5)     newptr[i] = (seq_t)542;
        else if (i == 6)     newptr[i] = (seq_t)1511;
        else if (i == 7)     newptr[i] = (seq_t)4144;
        else
        {
            seq_t t1 = newptr[i-2] * (seq_t)12;
            seq_t t2 = newptr[i-4] * (seq_t)38;
            seq_t t3 = newptr[i-6] * (seq_t)12;
            newptr[i] = t1 - t2 + t3 - newptr[i-8];
        }
    }

    cache = newptr;
    cache_cap = needed;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
