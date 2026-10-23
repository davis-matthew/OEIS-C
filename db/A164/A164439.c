/*
 * A164439  Number of binary strings of length n with no substrings equal to
 *          0000 0110 or 1001.
 * https://oeis.org/A164439
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+a(n-2)-a(n-3)+2*a(n-4)-a(n-5)+a(n-6) * with
 *            initial values: a(4)=13, a(5)=21, a(6)=34, a(7)=56, * a(8)=92,
 *            a(9)=151
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 43, u64 n <= 88, u128 n <= 178,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* static cache: cache[i] holds a(4+i) */
static seq_t *cache = NULL;
static size_t cap   = 0;   /* allocated size of the cache array */
static size_t stored = 0;  /* number of values already computed (next free index) */

/* Ensure that the cache has room for at least (needed+1) elements */
static void ensure_cap(size_t needed)
{
    if (cap <= needed) {
        size_t new_cap = cap ? cap * 2 : 16;
        while (new_cap <= needed)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();    /* allocation failure */
        cache = tmp;
        cap   = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n < 4)
        return (seq_t)0;   /* behavior for indices below the offset is irrelevant */

    /* first time: initialise the cache with the known initial values */
    if (stored == 0) {
        ensure_cap(5);      /* at least 6 elements needed (indices 0..5) */
        cache[0] = (seq_t)13;   /* a(4) */
        cache[1] = (seq_t)21;   /* a(5) */
        cache[2] = (seq_t)34;   /* a(6) */
        cache[3] = (seq_t)56;   /* a(7) */
        cache[4] = (seq_t)92;   /* a(8) */
        cache[5] = (seq_t)151;  /* a(9) */
        stored = 6;
    }

    /* target index in the cache array (0‑based) */
    size_t targetIdx = (size_t)(n - 4);

    /* if the requested value is already computed, return it */
    if (stored > targetIdx)
        return cache[targetIdx];

    /* compute successive terms until the target is reached */
    for (size_t idx = stored; idx <= targetIdx; ++idx) {
        ensure_cap(idx);   /* make sure we have space for this element */

        /* recurrence: a(n) = a(n-1) + a(n-2) - a(n-3) + 2*a(n-4)
                         - a(n-5) + a(n-6)   */
        seq_t val = cache[idx-1]               /* a(n-1) */
                 + cache[idx-2]               /* a(n-2) */
                 - cache[idx-3]               /* a(n-3) */
                 + (seq_t)2 * cache[idx-4]    /* 2*a(n-4) */
                 - cache[idx-5]               /* a(n-5) */
                 + cache[idx-6];              /* a(n-6) */
        cache[idx] = val;
        ++stored;
    }

    return cache[targetIdx];
}
