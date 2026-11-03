/*
 * A044417  Numbers n such that string 8,5 occurs in the base 10
 *          representation of n but not of n-1.
 * https://oeis.org/A044417
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Validated: partial, pass, 39 terms through n = 39 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A044417
   a(n) = n‑th positive integer whose decimal representation contains the substring
   "85" while the representation of n-1 does not. */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/mem.h"

/* Check whether a decimal representation of *x* contains "85". */
static bool contains85(seq_t x)
{
    seq_t tmp = x;
    unsigned int prev = 0xFFu;              /* sentinel for "no previous digit" */
    unsigned long long digit;

    while (tmp != 0)
    {
        /* tmp % 10U gives a seq_t; we convert that to unsigned long long
           so we can test against small constants. */
        digit = (unsigned long long)(tmp % 10U);
        tmp   = tmp / 10U;                  /* drop the least significant digit */

        if (prev == 5u && digit == 8u)
            return true;

        prev = (unsigned int)digit;
    }
    return false;
}

/* Public function required by the framework. */
seq_t a(idx_t n)
{
    /* Cached sequence values */
    static seq_t *cache   = NULL;
    static size_t cache_len = 0;   /* number of terms stored */
    static size_t cache_cap = 0;   /* allocated capacity */

    /* State of incremental scan */
    static seq_t last_checked = 0;
    static bool last_has85    = false;

    if (n == 0)
        return 0;                 /* undefined for offset 1, but harmless */

    /* Generate terms until we have at least n of them */
    while (cache_len < (size_t)n)
    {
        /* Increment the candidate number */
        seq_t cur = last_checked + 1;
        last_checked = cur;

        bool cur_has85 = contains85(cur);

        if (cur_has85 && !last_has85)
        {
            /* Grow cache if necessary */
            if (cache_len == cache_cap)
            {
                cache_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                cache = mem_resize(seq_t, cache, cache_cap);
            }
            cache[cache_len++] = cur;
        }
        last_has85 = cur_has85;
    }
    return cache[n - 1];
}
