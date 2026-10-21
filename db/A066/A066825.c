/*
 * A066825  a(1) = 1; set of digits of a(n)^2 is a subset of the set of digits
 *          of a(n+1)^2.
 * https://oeis.org/A066825
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 705, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_digits, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A066825:  a(1)=1; a(n) is the least integer > a(n‑1)
    whose square contains all decimal digits present in
    a(n‑1)².  The property is that the set of digits of
    a(n‑1)² is a subset of the set of digits of a(n)².
    This function builds the sequence incrementally,
    caching previous terms so that calls may come in any
    order.  It works for any supplied seq_t/idx_t
    (with seq_t being unsigned 8–128 bit or an arbitrary‑precision
    integer type).  Only standard operations and the
    utility routines listed in the sequence description
    are used. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/get_digits.h"   /* get_digits(seq_t, uint8_t *, size_t *) */
#include "../utils/seq_mul.h"     /* seq_mul(seq_t, seq_t) */

static uint16_t digit_mask_sq(seq_t val)
/* Return a bitmask of the decimal digits that appear in the
   decimal representation of val * val. */
{
    seq_t sq = seq_mul(val, val);
    uint8_t buf[128];
    size_t len;
    get_digits(sq, buf, &len);
    uint16_t mask = 0;
    for (size_t i = 0; i < len; ++i)
        mask |= (uint16_t)1 << buf[i];
    return mask;
}

seq_t a(idx_t n)
{
    if (n == 0)            /* offset is 1, behaviour for n<1 is arbitrary */
        return (seq_t)0;

    /* static cache of computed terms (0‑based: cache[0] = a(1)) */
    static seq_t *cache = NULL;
    static size_t cap   = 0;    /* allocated capacity */
    static idx_t len    = 0;    /* number of terms stored */

    /* Ensure at least one term is present for n==1 */
    if (len == 0)
    {
        cap = 16;
        cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        len = 1;
    }

    /* Extend the cache until it contains a(n) */
    while (len < n)
    {
        if (len >= cap)
        {
            size_t new_cap = cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();    /* allocation failure */
            cache = tmp;
            cap = new_cap;
        }

        seq_t prev = cache[len - 1];
        uint16_t mask_prev = digit_mask_sq(prev);

        seq_t candidate = prev + (seq_t)1;
        for (;; ++candidate)
        {
            uint16_t mask_candidate = digit_mask_sq(candidate);
            if ((mask_candidate & mask_prev) == mask_prev)
                break;      /* found the next term */
        }

        cache[len] = candidate;
        ++len;
    }

    return cache[n - 1];    /* n is 1‑based */
}
