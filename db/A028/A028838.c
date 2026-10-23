/*
 * A028838  Numbers whose sum of digits is a power of 2.
 * https://oeis.org/A028838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 51, u16 n <= 4574, u32 n <= 8513, u64 n <= 8513,
 *            u128 n <= 8513, big any n
 * Helpers:   digit_sum, is_power_of_two
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers whose sum of digits is a power of two
   a(1) = 1, a(2) = 2, a(3) = 4, a(4) = 8, …
   The function computes terms by searching integers in ascending order
   and caching the results.  This works for all supported seq_t types
   without requiring dynamic memory allocation, which keeps the code
   valid for both C and C++ compiles. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/digit_sum.h"
#include "../utils/is_power_of_two.h"

seq_t a(idx_t n)
{
    /* offset is 1 – return 0 for safety if called with n==0 */
    if (n == 0)
        return 0;

    /* cache of found terms – large enough for n up to about 10 000 */
    static seq_t cache[20000];
    static size_t cache_len = 0;
    static seq_t last_candidate = 0;   /* last integer tested */

    /* grow the cache until we have at least n terms */
    while (cache_len < n)
    {
        /* test next integer */
        ++last_candidate;   /* start from 1 and increase */

        if (is_power_of_two(digit_sum(last_candidate)))
        {
            /* store the new term */
            cache[cache_len++] = last_candidate;
        }
    }

    return cache[n - 1];
}
