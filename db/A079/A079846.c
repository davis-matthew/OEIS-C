/*
 * A079846  Powers of 3 which differ at all digits from the previous term.
 * https://oeis.org/A079846
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   get_digits, mul_small, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence computed by iterating powers of 3 and selecting those
   whose set of decimal digits is disjoint from the previous term's
   digit set. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"
#include "../utils/mul_small.h"
#include "../utils/get_digits.h"

seq_t a(idx_t n) {
    /* static cache of found terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;          /* number of terms stored */
    static seq_t pw = (seq_t)1;           /* current power of 3 */
    static uint16_t prev_mask = 0;        /* digit mask of last term */

    /* initialisation */
    if (cache == NULL) {
        cache_cap = 8;
        cache = seq_alloc(cache_cap);
        cache[0] = (seq_t)1;
        cache_len = 1;
        prev_mask = (1u << 1);            /* digit '1' of the first term */
    }

    /* generate terms until we have at least n+1 terms */
    while (cache_len <= (size_t)n) {
        /* next power of 3 */
        pw = mul_small(pw, (unsigned int)3);

        /* obtain digit mask of pw */
        static uint8_t digit_buf[20000];  /* big enough for the needed range */
        size_t len;
        get_digits(pw, digit_buf, &len);
        uint16_t mask = 0;
        for (size_t i = 0; i < len; ++i) {
            mask |= (1u << digit_buf[i]);
        }

        /* accept this term if its digit mask is disjoint from the previous one */
        if ((mask & prev_mask) == 0) {
            if (cache_len == cache_cap) {
                cache_cap *= 2;
                cache = seq_realloc(cache, cache_len, cache_cap);
            }
            cache[cache_len++] = pw;
            prev_mask = mask;
        }
    }

    return cache[(size_t)n];
}
