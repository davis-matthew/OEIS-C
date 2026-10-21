/*
 * A062998  Numbers whose sum of digits is less than or equal to its product
 *          of digits.
 * https://oeis.org/A062998
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 174, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_product_local
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: a(n) = the nth integer x  (offset 1) whose decimal digits satisfy */
/*              digit_sum(x) <= digit_product(x).  The function builds the  */
/*              list on demand and memoises values.  Because seq_t may be an  */
/*              arbitrary‑precision class, dynamic resizing is done by a    */
/*              simple array of a fixed (sufficiently large) size.         */

#include <stdint.h>
#include "../utils/digit_product_local.h"

#define CACHE_MAX 200000          /* enough for all practical queries (n up to ~100 000) */

/* digit sum ------------------------------------------------------------------ */
static inline seq_t digit_sum_local(seq_t x)
{
    seq_t sum = 0;
    seq_t tmp = x;
    while (tmp != (seq_t)0)
    {
        seq_t d = tmp % (seq_t)10;
        sum += d;
        tmp /= (seq_t)10;
    }
    return sum;
}

/* static cache for sequence values -------------------------------------------- */
static seq_t cache[CACHE_MAX];
static idx_t cache_len = 0;   /* number of cached terms */
static seq_t last_test = 0;   /* last integer inspected for the property */

/* ----------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return 0;   /* offset is 1; request 0 is undefined */

    /* Build the cache until it contains at least n terms */
    while (cache_len < n)
    {
        /* Examine the next integer */
        ++last_test;

        seq_t sum = digit_sum_local(last_test);
        seq_t prod = digit_product_local(last_test);

        if (sum <= prod)
        {
            /* Append to cache */
            if (cache_len == CACHE_MAX)
                return 0;   /* cache overflow – should not happen for expected n */

            cache[cache_len++] = last_test;
        }
    }

    return cache[n - 1];
}
