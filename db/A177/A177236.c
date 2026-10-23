/*
 * A177236  a(n) = A049820(n) - A177235(n).
 * https://oeis.org/A177236
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of k with 1<k<n, n%k!=0, and floor(n/k) even
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t k = 2; k < n; ++k)
    {
        if (n % k != 0)
        {
            idx_t q = n / k;          /* floor(n/k) */
            if ((q & 1) == 0)         /* q even    */
                ++count;
        }
    }
    return count;
}
