/*
 * A230354  Even numbers n such that digit sum of n = digit sum of largest odd
 *          divisor of n.
 * https://oeis.org/A230354
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   digit_sum, ensure_capacity, odd_part
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A230354: even numbers n with digit_sum(n)=digit_sum(odd_part(n))
    a(1)=12, a(2)=18, a(3)=36, ...  */

#include <stddef.h>
#include "../utils/odd_part.h"
#include "../utils/digit_sum.h"
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n)
{
    /*  static cache of previously found terms  */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    /*  next even integer to test (starts at 2)  */
    static seq_t next_even = 2;

    /*  extend cache until we have at least n terms  */
    while (cache_len < n)
    {
        seq_t cand = next_even;
        seq_t odd  = odd_part(cand);

        if (digit_sum(cand) == digit_sum(odd))
        {
            /*  ensure enough space and store the term  */
            if (cache_len + 1 > cache_cap)
                ensure_capacity(cache_len + 1, &cache_cap,
                                (void **)&cache, sizeof(seq_t));
            cache[cache_len++] = cand;
        }

        next_even += 2;   /*  next even integer  */
    }

    return cache[n - 1];
}
