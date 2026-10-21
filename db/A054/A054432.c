/*
 * A054432  a(n) = Sum_{1<=k<=n, gcd(k,n)=1} 2^(k-1).
 * https://oeis.org/A054432
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{1 <= k <= n, gcd(k,n)=1} 2^(k-1)
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   local_gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/local_gcd_u64.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)          /* OEIS offset starts at 1 */
        return (seq_t)0;

    seq_t sum   = (seq_t)0;          /* cumulative sum   */
    seq_t power = (seq_t)1;          /* 2^(k-1)   */

    for (idx_t k = 1; k <= n; ++k) {
        if (local_gcd_u64((uint64_t)k, (uint64_t)n) == 1)
            sum += power;           /* add 2^(k-1) when gcd(k, n) == 1 */
        power *= (seq_t)2;          /* prepare next power of two */
    }
    return sum;
}
