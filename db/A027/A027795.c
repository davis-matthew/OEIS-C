/*
 * A027795  a(n) = 15*(n+1)*binomial(n+3,10).
 * https://oeis.org/A027795
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 15*(n+1)*binomial(n+3,10)
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 24, u64 n <= 175,
 *            u128 n <= 1000, big any n
 * Helpers:   binom10_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom10_u64.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 7; values for smaller n are
     * irrelevant for the tests but are still computed correctly. */
    seq_t bin = binom10_u64(n + 3);          /* C(n+3,10)       */
    seq_t ans = bin * (n + 1);               /* bin * (n+1)     */
    ans *= 15;                               /* final result    */
    return ans;
}
