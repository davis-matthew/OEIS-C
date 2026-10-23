/*
 * A021729  Decimal expansion of 1/725.
 * https://oeis.org/A021729
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(0)=0, a(1)=0, a(2)=1, a(3)=3, a(4)=7, a(5)=9, a(6)=3, a(7)=1,
 *            a(8)=0, a(9)=3, a(10)=4, a(11)=4, a(12)=8, a(13)=2, a(14)=7,
 *            a(15)=5, a(16)=8, a(n)=a(n-1)-a(n-14)+a(n-15) for n>16
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint8_t *digit_cache = NULL;     /* cache of computed digits */
static size_t cache_cap   = 0;          /* allocated capacity */
static size_t cache_len   = 0;          /* number of digits stored */

/* initial 17 digits of 1/725 */
static const uint8_t init_digits[17] =
{
    0,0,1,3,7,9,3,1,0,3,4,4,8,2,7,5,8
};

seq_t a(idx_t n)
{
    /* if n is small, return from pre‑initialized table immediately */
    if (n < 17)
        return (seq_t) init_digits[(size_t)n];

    /* required size */
    size_t req = (size_t)n + 1;

    /* first time: allocate and copy the initial 17 digits */
    if (cache_len == 0) {
        /* ensure capacity at least req and at least 17 */
        if (cache_cap < req) cache_cap = (req > 17 ? req : 17);
        cache_cap *= 2;                      /* start doubling for future growth */
        digit_cache = (uint8_t *)malloc(cache_cap * sizeof(uint8_t));
        if (!digit_cache) abort();           /* allocation failure */
        for (size_t i = 0; i < 17; ++i) digit_cache[i] = init_digits[i];
        cache_len = 17;
    }

    /* grow the cache if necessary */
    if (req > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap < req) new_cap *= 2;
        uint8_t *tmp = (uint8_t *)realloc(digit_cache, new_cap * sizeof(uint8_t));
        if (!tmp) abort();
        digit_cache = tmp;
        cache_cap = new_cap;
    }

    /* compute new digits up to required index */
    for (size_t i = cache_len; i < req; ++i) {
        int val = (int)digit_cache[i-1]
                - (int)digit_cache[i-14]
                + (int)digit_cache[i-15];
        digit_cache[i] = (uint8_t)val;
    }
    cache_len = req;

    return (seq_t) digit_cache[(size_t)n];
}
