/*
 * A229937  Nonprime odious numbers.
 * https://oeis.org/A229937
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: find the nth nonprime odious number
   (intersection of nonprime numbers and numbers with an odd
   number of 1's in binary representation). */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

/* popcount of an idx_t value */
static inline unsigned popcount_idx(idx_t x)
{
    unsigned c = 0;
    while (x)
    {
        x &= x - 1;  /* flip lowest set bit */
        ++c;
    }
    return c;
}

seq_t a(idx_t n)
{
    idx_t count = 0;   /* how many terms found so far */
    idx_t cand   = 1;  /* current candidate to test */

    while (true)
    {
        /* candidate must be nonprime (1 is treated as nonprime) */
        if (!is_prime(cand))
        {
            /* odious: odd number of 1's in binary */
            if (popcount_idx(cand) & 1)
            {
                ++count;
                if (count == n)
                    return (seq_t)cand;
            }
        }
        ++cand;
    }
}
