/*
 * A062398  Numbers such that the product of the digits and the sum of the
 *          digits are squares.
 * https://oeis.org/A062398
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_product, digit_sum, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A062398: numbers for which the sum of the decimal digits and the product of the decimal digits are both perfect squares.
   a(idx_t n) returns the n-th such integer (1–based).  The function scans all positive integers, keeps a static cache of the
   qualifying numbers, and grows the cache as needed.  The external utilities digit_sum, digit_product and floor_sqrt are used
   to compute the required properties. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_sum.h"
#include "../utils/digit_product.h"
#include "../utils/floor_sqrt.h"

/* Return true if v is a perfect square. */
static bool is_square(seq_t v)
{
    seq_t r = floor_sqrt(v);
    return r * r == v;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *cache     = NULL;
    static size_t cache_cap = 0;   /* allocated capacity   */
    static size_t cache_len = 0;   /* number of terms stored  */
    static seq_t next_candidate = (seq_t)0; /* next integer to test */

    /* Grow the cache until it contains at least n terms */
    while (cache_len < (size_t)n)
    {
        /* grow the array if full */
        if (cache_len == cache_cap)
        {
            size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
            void *tmp = realloc(cache, new_cap * sizeof(seq_t));
            cache = (seq_t *)tmp;
            cache_cap = new_cap;
        }

        /* test next integer */
        next_candidate += (seq_t)1;               /* start at 1 */
        seq_t sum  = digit_sum(next_candidate);
        seq_t prod = digit_product(next_candidate);
        if (is_square(sum) && is_square(prod))
        {
            cache[cache_len++] = next_candidate;
        }
    }

    /* return the requested term (1‑based index) */
    return cache[(size_t)(n - 1)];
}
