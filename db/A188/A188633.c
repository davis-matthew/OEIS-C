/*
 * A188633  Numbers of the form 2^k * m, with k > 1 and m an odd composite
 *          number.
 * https://oeis.org/A188633
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_odd_composite
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute OEIS A188633.
   Numbers of the form 2^k * m, with k > 1 and m an odd composite number.
   This implementation iterates over integers, tests the condition,
   and returns the n-th qualifying number. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_odd_composite.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined, but guard against invalid input */

    idx_t count = 0;    /* number of terms found so far */
    seq_t candidate = 4; /* start from the smallest multiple of 4 */

    for (;; ++candidate)
    {
        /* candidate must be divisible by 4 (k > 1) */
        if ((candidate & 3) != 0) continue;

        /* strip all factors of 2 to obtain the odd part m */
        seq_t m = candidate;
        while ((m & 1) == 0) m >>= 1;

        /* m must be odd composite */
        if (m > 1 && is_odd_composite(m))
        {
            ++count;
            if (count == n) return candidate;
        }
    }
}
