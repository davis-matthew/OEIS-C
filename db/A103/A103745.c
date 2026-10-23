/*
 * A103745  a(n) = (A102371(n) + n)/2.
 * https://oeis.org/A103745
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k>=1, k ≡ n (mod 2^k)} 2^(k-1)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* value below the offset is irrelevant */

    seq_t sum = 0;

    /* iterate over k for which 2^k ≤ n */
    for (idx_t k = 1; ( (uint64_t)1 << k ) <= n; ++k)
    {
        if ( n % ( (idx_t)1 << k ) == k )
            sum += pow2(k - 1);
    }

    /* the term for k = n always satisfies the condition */
    sum += pow2(n - 1);

    return sum;
}
