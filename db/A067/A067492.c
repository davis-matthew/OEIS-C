/*
 * A067492  Powers of 6 with initial digit 1.
 * https://oeis.org/A067492
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 17,
 *            big any n
 * Helpers:   digit_len, pow10, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A067492: powers of 6 whose leading decimal digit is 1 (a(1)=1). */
/* The function searches for successive powers of 6 and caches the ones
   with leading digit 1.  It works for any seq_t size and does not
   assume monotonically increasing calls. */

#include <stdlib.h>
#include "../utils/seq_mul.h"
#include "../utils/pow10.h"
#include "../utils/digit_len.h"

seq_t a(idx_t n)
{
    /* Static cache for the discovered terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* capacity of cache in elements */
    static size_t cache_len = 0;   /* number of stored terms */

    /* Current power of 6 (6^exp) and its exponent */
    static seq_t cur_val = 0;  /* will be set to 1 on first call */
    static idx_t cur_exp  = 0; /* exponent of cur_val */

    /* Lazy initialization */
    if (cache == NULL) {
        cache_cap = 64;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        cur_val = (seq_t)1;          /* 6^0 */
        cur_exp = 0;
        cache[0] = cur_val;          /* a(1) = 1 */
        cache_len = 1;
    }

    /* Generate terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        /* Advance to next power of 6 */
        cur_val = seq_mul(cur_val, (seq_t)6);
        ++cur_exp;

        /* Check whether the leading decimal digit is 1 */
        idx_t   d   = digit_len(cur_val);           /* number of decimal digits */
        seq_t   base = pow10(d - 1);                /* 10^(d-1) */
        seq_t   leading = cur_val / base;           /* leading digit */

        if (leading == (seq_t)1) {
            /* Ensure capacity */
            if (cache_len == cache_cap) {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, sizeof(seq_t) * cache_cap);
            }
            cache[cache_len++] = cur_val;
        }
    }

    return cache[n - 1];
}
