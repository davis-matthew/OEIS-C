/*
 * A023383  a(n+1) = a(n) converted to base 10 from base 5 (written in base
 *          10).
 * https://oeis.org/A023383
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n+1) = decimal value of a(n) after interpreting a(n) as a
 *            base‑5 number. Initial condition: a(1) = 5
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 9, u64 n <= 11, u128 n <= 13,
 *            big any n
 * Validated: standard, pass, 22 terms through n = 22 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

/* Convert a number written in base‑5 (digits 0‑4) to its
   base‑10 equivalent.  Works for arbitrary‑precision seq_t
   as well as fixed‑width types. */
static seq_t base5_to_dec(seq_t x)
{
    if (x == (seq_t)0)
        return (seq_t)0;

    /* Collect base‑5 digits (least significant first). */
    size_t cap = 16;
    uint8_t *digits = (uint8_t *)malloc(cap * sizeof(uint8_t));
    if (!digits)
        abort();

    size_t len = 0;
    while (x != (seq_t)0)
    {
        seq_t rem = x % (seq_t)5;
        if (len == cap)
        {
            cap <<= 1;
            uint8_t *tmp = (uint8_t *)realloc(digits, cap * sizeof(uint8_t));
            if (!tmp)
                abort();
            digits = tmp;
        }
        digits[len++] = (uint8_t)rem;
        x /= (seq_t)5;
    }

    /* Reconstruct decimal value from most significant digit. */
    seq_t y = (seq_t)0;
    for (size_t i = len; i > 0; --i)
        y = y * (seq_t)10 + (seq_t)digits[i - 1];

    free(digits);
    return y;
}

static seq_t *cache = NULL;    /* cache[0] = a(1) */
static size_t cache_len = 0;   /* number of cached terms */
static size_t cache_cap = 0;   /* capacity of the cache */

seq_t a(idx_t n)
{
    if (n == 0)   /* out of bounds, should not occur */
        return (seq_t)0;

    /* Initialise cache on first call. */
    if (cache_len == 0)
    {
        cache_cap = 8;
        cache = mem_new(seq_t, cache_cap);
        if (!cache)
            abort();
        cache[0] = (seq_t)5;   /* a(1) */
        cache_len = 1;
    }

    /* Extend cache up to requested index. */
    while (cache_len < n)
    {
        seq_t last = cache[cache_len - 1];
        seq_t next = base5_to_dec(last);

        if (cache_len == cache_cap)
        {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = mem_resize(seq_t, cache, new_cap);
            if (!tmp)
                abort();
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_len] = next;
        cache_len++;
    }

    return cache[n - 1];
}
