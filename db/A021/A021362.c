/*
 * A021362  Decimal expansion of 1/358.
 * https://oeis.org/A021362
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: decimal digits of 1/358 (OEIS A021362) */
/* a(n) = (the nth digit after the decimal point in 1/358) */

#include <stdlib.h>

static seq_t *digit_cache = NULL;   /* array of computed digits */
static size_t cache_len  = 0;       /* number of digits stored so far */
static size_t cache_cap  = 0;       /* allocated capacity of the array */
static seq_t  rem        = 1;       /* current remainder in the long division */

/* ensure the cache has capacity for at least need digits */
static void grow_cache(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap : 8;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(digit_cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        /* allocation failure – terminate the program */
        abort();
    }
    digit_cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* compute digits up to index n, if not already done */
    size_t target = (size_t)n;
    if (target >= cache_len) {
        grow_cache(target + 1);
        const seq_t denom = 358;
        for (size_t i = cache_len; i <= target; ++i) {
            seq_t d = (rem * 10) / denom;          /* digit */
            rem  = (rem * 10) % denom;            /* new remainder */
            digit_cache[i] = d;
        }
        cache_len = target + 1;
    }
    return digit_cache[target];
}
