/*
 * A087047  a(n) = n*(n+1)*(n+2)*a(n-1)/6 for n >= 1; a(0) = 1.
 * https://oeis.org/A087047
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = product_{k=1..n} k*(k+1)*(k+2)/6 with a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t result = (seq_t)1;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t kk = (seq_t)k;
        /* compute the k-th tetrahedral number: kk*(kk+1)*(kk+2)/6 */
        seq_t t = kk * (kk + 1) * (kk + 2) / (seq_t)6;
        result *= t;
    }
    return result;
}
