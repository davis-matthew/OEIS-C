/*
 * A044293  Numbers n such that string 4,6 occurs in the base 9 representation
 *          of n but not of n-1.
 * https://oeis.org/A044293
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th positive integer whose base‑9 representation *
 *            contains the substring “46” but the integer n‑1 does not. *
 *            Offset: 1 (a(1)=42)
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Validated: partial, pass, 39 terms through n = 39 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>                     /* abort() */
#include <stddef.h>
#include "../utils/mem.h"

static seq_t *alloc_seq_array(size_t n)
{
    return mem_new(seq_t, n);
}

static void free_seq_array(seq_t *p)
{
    mem_free(p);
}

/* Return true iff x has the adjacent digits 4,6 in base‑9. */
static bool has46(seq_t x)
{
    if (x == (seq_t)0)
        return false;

    /* 64‑bit numbers have at most 22 base‑9 digits; use a small fixed array. */
    uint8_t d[32];
    int len = 0;
    seq_t tmp = x;
    while (tmp != (seq_t)0)
    {
        d[len++] = (uint8_t)(tmp % (seq_t)9);
        tmp /= (seq_t)9;
    }

    for (int i = len - 1; i > 0; --i)
        if (d[i] == 4 && d[i - 1] == 6)
            return true;
    return false;
}

/* Static cache of computed terms. */
static seq_t *cache      = NULL;   /* array of terms */
static idx_t  cache_cap   = 0;      /* allocated capacity   */
static idx_t  cache_len   = 0;      /* terms stored so far */
static seq_t  cur         = (seq_t)1;   /* next integer to test   */
static bool   prev_has    = false;     /* has46(cur‑1) flag */

/* Ensure that at least n terms are stored in the cache. */
static void ensure_cache(idx_t n)
{
    /* Grow storage if needed. */
    if (cache_cap < n)
    {
        idx_t new_cap = n * 2;
        seq_t *tmp = alloc_seq_array(new_cap);
        if (!tmp)
            abort();                    /* allocation failure */

        /* Copy existing terms. */
        for (idx_t i = 0; i < cache_len; ++i)
            tmp[i] = cache[i];

        /* Release old storage. */
        if (cache)
            free_seq_array(cache);

        cache = tmp;
        cache_cap = new_cap;
    }

    /* Generate terms until we have n of them. */
    while (cache_len < n)
    {
        bool now_has = has46(cur);
        if (now_has && !prev_has)
            cache[cache_len++] = cur;
        prev_has = now_has;
        cur += (seq_t)1;
    }
}

/* Main sequence routine.  Returns the n‑th term (1‑based). */
seq_t a(idx_t n)
{
    if (n < (idx_t)1)
        return (seq_t)0;   /* undefined for n=0, but harmless */

    ensure_cache(n);
    return cache[n - 1];
}
