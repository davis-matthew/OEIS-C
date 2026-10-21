/*
 * A056893  Smallest prime with square excess of n.
 * https://oeis.org/A056893
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest prime p such that p = k^2 + n where k =
 *            floor(sqrt(p)). That is, the square excess of p is n
 * Range:     u8 n <= 16, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    /* If n is less than the offset (1) the value is undefined;
       we simply return 0 for safety. */
    if (n == 0)
        return 0;

    /* The minimal integer k that can satisfy n < 2k + 1 is
       ceil((n + 1) / 2).  Using integer division gives the correct
       result for all n >= 1. */
    seq_t k = (seq_t)(n + 1) / 2;

    for (;;) {
        seq_t candidate = k * k + n;     /* p = k^2 + n   */
        if (isPrimeGeneric(candidate))
            return candidate;           /* found the smallest prime with square excess n */
        ++k;                            /* try next k value   */
    }
}
