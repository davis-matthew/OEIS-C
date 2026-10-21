/*
 * A052224  Numbers whose sum of digits is 10.
 * https://oeis.org/A052224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 962, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers whose sum of digits is 10
   a(n) = the n‑th positive integer whose digit sum equals 10
   (computed via a simple linear scan). */
#include <stdint.h>          /* for uint64_t */
#include "../utils/digit_sum_u64.h"

/* External linkage as required. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;          /* undefined for n = 0 */

    idx_t  found    = 0;           /* how many terms we have found */
    uint64_t candidate = 1;       /* current number to test */

    while (1)
    {
        if (digit_sum_u64(candidate) == 10U)
        {
            ++found;
            if (found == n)
                return (seq_t)candidate;
        }
        ++candidate;
    }
}
