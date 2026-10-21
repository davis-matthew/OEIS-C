/*
 * A081649  Integers congruent to 0, 1, 4, 9, 16, 25, 36, 49, 64 or 81 (mod
 *          100) which are not squares.
 * https://oeis.org/A081649
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A081649
    a(n) = the n-th integer congruent to 0,1,4,9,16,25,36,49,64,81 (mod 100)
    that is NOT a perfect square.
    Algorithm: iterate over integers, test the last two digits and check
    for squareness with floor_sqrt.  An O(n) search suffices for the
    required range. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

static inline bool is_not_square(seq_t v)
{
    seq_t s = floor_sqrt(v);
    return s * s != v;
}

seq_t a(idx_t n)
{
    /* n ≥ 1; behaviour for n==0 is undefined */
    idx_t count = 0;
    idx_t i = 1;
    for (;; ++i)
    {
        uint32_t r = i % 100;          /* last two digits */
        switch (r)
        {
            case 0: case 1: case 4: case 9: case 16:
            case 25: case 36: case 49: case 64: case 81:
                if (is_not_square((seq_t)i))
                {
                    ++count;
                    if (count == n)
                        return (seq_t)i;
                }
                break;
        }
    }
}
