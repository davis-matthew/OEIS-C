/*
 * A061514  a(0) = 0; a(n) is obtained by incrementing each digit of a(n-1) by
 *          3.
 * https://oeis.org/A061514
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(0) = 0, a(n) = inc_by3(a(n-1)) where inc_by3 increments each
 *            decimal digit of x by 3, writing the result in decimal, with
 *            digits that exceed 9 written as two decimal digits (e.g., 9+3 →
 *            "12")
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 12, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Validated: partial, pass, 19 terms through n = 18 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Helper: apply the digit‑increment rule once. */
static seq_t inc_by3(seq_t x)
{
    /* Special case for 0 – its decimal digit "0" becomes "3". */
    if (x == (seq_t)0)
        return (seq_t)3;

    seq_t result = (seq_t)0;
    seq_t pow10  = (seq_t)1;
    while (x != (seq_t)0) {
        seq_t d     = x % (seq_t)10;
        x          /= (seq_t)10;
        seq_t nv    = d + (seq_t)3;
        if (nv < (seq_t)10) {
            result += nv * pow10;
            pow10   *= (seq_t)10;
        } else {
            /* nv is 10…12 – write two digits. */
            seq_t low  = nv % (seq_t)10;
            seq_t high = nv / (seq_t)10;
            result += low * pow10;
            pow10   *= (seq_t)10;
            result += high * pow10;
            pow10   *= (seq_t)10;
        }
    }
    return result;
}

/* Cache the sequence values to allow random access and reuse. */
static seq_t *cache = NULL;
static idx_t cache_len = 0;   /* number of elements stored (>=1 if cache!=NULL) */
static size_t cache_cap = 0;  /* allocated capacity */

/* Grow the cache if necessary. */
static void maybe_grow(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = (cache_cap == 0) ? 1 : cache_cap * 2;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *new_cache = mem_resize(seq_t, cache, new_cap);
    cache          = new_cache;
    cache_cap      = new_cap;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (cache_len == 0) {          /* first call: initialise cache[0] */
        maybe_grow(1);
        cache[0] = (seq_t)0;
        cache_len = 1;
    }

    if (n < cache_len)
        return cache[n];

    /* Compute successive terms up to n. */
    for (idx_t i = cache_len; i <= n; ++i) {
        maybe_grow(i + 1);
        cache[i] = inc_by3(cache[i - 1]);
    }
    cache_len = n + 1;
    return cache[n];
}
