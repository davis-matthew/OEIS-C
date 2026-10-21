/*
 * A067490  Powers of 4 with initial digit 1.
 * https://oeis.org/A067490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 23,
 *            big any n
 * Helpers:   digit_len, mul_small, pow10, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the A067490 sequence: powers of 4 whose decimal representation
   starts with the digit 1.  a(1) = 1, a(2) = 16, a(3) = 1024, ... */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"
#include "../utils/mul_small.h"
#include "../utils/pow10.h"
#include "../utils/digit_len.h"

static void ensure_cache_capacity(idx_t need,
                                  seq_t **cache, size_t *cap)
{
    size_t required = (size_t)need;
    if (*cache == NULL) {
        *cap = required > 8 ? required * 2 : 8;
        *cache = seq_alloc(*cap);
    } else if (*cap < required) {
        size_t new_cap = *cap * 2;
        if (new_cap < required)
            new_cap = required;
        seq_t *tmp = seq_realloc(*cache, *cap, new_cap);
        if (tmp == NULL)
            return;  /* allocation failed – caller will see incomplete cache */
        *cache = tmp;
        *cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;        /* Undefined for n < 1 */

    /* Static cache of observed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* Current exponent of the running power 4^k and its value */
    static idx_t current_exp = 0;
    static seq_t current_power = (seq_t)1;   /* 4^0 */

    /* Initialise cache on first call */
    if (cache_len == 0) {
        ensure_cache_capacity(1, &cache, &cache_cap);
        cache[0] = current_power;  /* 4^0 = 1 */
        cache_len = 1;
    }

    /* Grow cache until it contains at least n terms */
    while (cache_len < n) {
        /* Advance to next exponent */
        current_exp++;
        current_power = mul_small(current_power, (unsigned int)4);

        /* Determine if 4^current_exp starts with digit 1 */
        size_t len = digit_len(current_power);
        seq_t divisor = pow10((idx_t)(len - 1));   /* 10^(len-1) */
        seq_t high_digit = current_power / divisor;

        if (high_digit == (seq_t)1) {
            /* Store this term */
            ensure_cache_capacity(cache_len + 1, &cache, &cache_cap);
            cache[cache_len] = current_power;
            cache_len++;
        }
    }

    return cache[n - 1];
}
