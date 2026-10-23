/*
 * A134738  Cubes which are not the sum of three squares.
 * https://oeis.org/A134738
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A004215(n)^3 where A004215(n) is the nth positive integer
 *            of the form 4^k·(8m+7), i.e. the numbers that cannot be
 *            expressed as a sum of three squares
 * Range:     u8 none, u16 n <= 6, u32 n <= 268, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_not_sum_of_three_squares
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_not_sum_of_three_squares.h"

/* Cache for A004215: store the numbers that are not sums of three squares
   in increasing order.  The cache is grown as needed. */
static seq_t *base_cache   = NULL;   /* values of A004215 */
static size_t base_len     = 0;      /* number of cached values */
static size_t base_cap     = 0;      /* allocated capacity */
static idx_t  checked_upto = 0;      /* largest integer examined so far */

seq_t a(idx_t n)
{
    /* Ensure at least n values are cached. */
    while (base_len < n)
    {
        ++checked_upto;
        if (is_not_sum_of_three_squares(checked_upto))
        {
            /* Grow cache when full. */
            if (base_len == base_cap)
            {
                size_t new_cap = base_cap ? (base_cap << 1) : 64;
                seq_t *tmp = (seq_t *)realloc(base_cache, new_cap * sizeof (seq_t));
                if (!tmp) abort();   /* memory failure */
                base_cache = tmp;
                base_cap   = new_cap;
            }
            base_cache[base_len++] = (seq_t)checked_upto;
        }
    }

    /* Retrieve the base value and cube it. */
    seq_t base = base_cache[(size_t)(n - 1)];
    seq_t result = base;
    result *= base;
    result *= base;
    return result;
}
