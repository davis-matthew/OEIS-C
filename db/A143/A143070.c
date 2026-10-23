/*
 * A143070  A positive integer n is included if the number of 0's in the
 *          binary representation of n is a power of 2 (including being
 *          possibly 1).
 * https://oeis.org/A143070
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 140, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_power_of_two_zeros
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A143070:
 * a(n) = n-th positive integer whose binary representation contains
 * a power‑of‑two number of zero bits (including 1).
 * Offset = 1 (a(1) = 2).
 */

#include <stdbool.h>
#include "../utils/has_power_of_two_zeros.h"

/* required function */
seq_t a(idx_t n)
{
    if (n == 0)            /* offset is 1 – not defined for n=0 */
        return 0;

    idx_t found = 0;       /* how many terms we have found */
    seq_t cand = 2;        /* candidate numbers start at 2 */

    while (found < n)
    {
        if (has_power_of_two_zeros(cand))
        {
            ++found;
            if (found == n)
                return cand;
        }
        cand++;            /* move to next candidate */
    }

    /* should never reach here */
    return 0;
}
