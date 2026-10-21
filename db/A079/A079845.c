/*
 * A079845  Powers of 2 which differ at all digits from the previous term.
 * https://oeis.org/A079845
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 *  A079845: Powers of 2 whose decimal digits differ from the previous term.
 *  Algorithm:  iterate powers of two (1,2,4,8,…) and keep a term when its
 *  set of decimal digits does not intersect the set of digits of the
 *  last accepted term.
 *
 *  The function caches all computed terms to allow arbitrary non‑monotonic
 *  access patterns and works for any build where seq_t supports unsigned
 *  integer arithmetic, bitwise operators, division by 10 and multiplication
 *  by 2.
 */
#include <stdint.h>
#include <stdlib.h>

/* Helper: compute the bitmask of decimal digits of a number */
static unsigned digit_mask(seq_t v)
{
    unsigned mask = 0;
    if (v == 0) return 1u;          /* only digit 0 */
    while (v != 0) {
        unsigned d = (unsigned)(v % 10);
        mask |= 1u << d;
        v /= 10;
    }
    return mask;
}

seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *cache      = NULL;
    static size_t  cache_sz  = 0;
    static size_t  cache_cap = 0;
    static seq_t  cur_power   = 0;   /* current power of two being examined */
    static unsigned last_mask = 0;
    static int     initialized = 0;

    if (!initialized) {
        /* initialise with the first term 2^0 = 1 */
        cache_cap = 8;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;   /* allocation failed */

        cur_power = 1;                          /* 2^0 */
        last_mask = digit_mask(cur_power);
        cache[0]  = cur_power;
        cache_sz  = 1;
        initialized = 1;
    }

    /* generate terms until we have at least n of them */
    while (cache_sz < (size_t)n) {
        /* stop if multiplication would wrap around (happens on 64‑bit build) */
        if (cur_power == 0) break;
        cur_power = cur_power * 2;

        unsigned mask = digit_mask(cur_power);
        if ((mask & last_mask) == 0) {          /* disjoint digit sets */
            if (cache_sz == cache_cap) {
                cache_cap <<= 1;
                seq_t *tmp = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp) { free(cache); cache = NULL; return 0; }
                cache = tmp;
            }
            cache[cache_sz++] = cur_power;
            last_mask = mask;
        }
    }

    if (n == 0 || cache_sz < (size_t)n) return 0;   /* out of range */
    return cache[n - 1];
}
