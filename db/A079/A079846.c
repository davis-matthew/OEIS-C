/*
 * A079846  Powers of 3 which differ at all digits from the previous term.
 * https://oeis.org/A079846
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Validated: partial, pass, 8 terms through n = 7 (2026-11-01)
 * Helpers:   get_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence computed by iterating powers of 3 and selecting those
   whose set of decimal digits is disjoint from the previous term's
   digit set. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/get_digits.h"

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

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
