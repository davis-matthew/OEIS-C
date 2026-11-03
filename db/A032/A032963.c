/*
 * A032963  Numbers whose base-13 representation Sum_{i=0..m} d(i)*13^(m-i)
 *          has even d(i) for all odd i.
 * https://oeis.org/A032963
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers whose base‑13 representation has even digits at all odd
 *            positions
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Validated: partial, pass, 62 terms through n = 62 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

/* Test whether a number has even digits on all odd positions in base‑13
   (MSB has index 0). */
static bool valid_base13(seq_t x)
{
    /* Store the digits in a temporary array. 64 digits are abundant for
       all numbers we generate for n up to 10 000.  This limit is safe for
       64‑bit values and is comfortably below any reasonable growth for
       the sequence. */
    uint8_t digits[64];
    int len = 0;
    seq_t t = x;

    /* Extract digits least‑significant‑first. */
    while (t > (seq_t)0)
    {
        digits[len++] = (uint8_t)(t % (seq_t)13);
        t /= (seq_t)13;
    }

    /* Zero never occurs for sequence terms, but a zero would trivially
       satisfy the condition. */
    if (len == 0)
        return true;

    /* Reverse to most‑significant‑first. */
    for (int i = 0; i < len / 2; ++i)
    {
        uint8_t tmp = digits[i];
        digits[i] = digits[len - 1 - i];
        digits[len - 1 - i] = tmp;
    }

    /* Check that every digit at an odd index is even (digits 0,2,4,6,8,10,12). */
    for (int i = 0; i < len; ++i)
    {
        if ((i & 1) && (digits[i] & 1))
            return false;
    }

    return true;
}

/* Return the n‑th element of sequence A032963 (offset 1). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;      /* offset is 1; behaviour for n==0 is irrelevant */

    /* Static cache of generated terms.  The cache is grown only as needed. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t next = 1;    /* the next integer to test */

    /* Extend the cache until it contains at least n terms. */
    while (cache_len < (size_t)n)
    {
        /* Find next valid integer. */
        while (!valid_base13(next))
            ++next;

        /* Grow the cache if necessary. */
        if (cache_len == cache_cap)
        {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            seq_t *tmp = mem_resize(seq_t, cache, new_cap);
            if (!tmp)
                abort();        /* out of memory */
            cache = tmp;
            cache_cap = new_cap;
        }

        cache[cache_len++] = next;
        ++next;
    }

    return cache[n - 1];
}
