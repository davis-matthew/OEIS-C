/*
 * A052203  a(n) = (4n+1)*binomial(4n,n)/(3n+1).
 * https://oeis.org/A052203
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = C(4n+1 , n) (the central binomial coefficient for 4n+1)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   binomial_custom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial_custom.h"

seq_t a(idx_t n)
{
    /* Compute C(4n+1 , n) directly.  This is O(n) and stays well below
       the time limit for n up to thousands. */
    idx_t m = 4 * n + 1;
    return binomial_custom(m, n);
}
