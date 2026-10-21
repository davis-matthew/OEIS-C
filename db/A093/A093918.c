/*
 * A093918  a(2*k-1) = (2*k-1)^2+k, a(2*k) = 6*k^2+k+1: Last term in rows of
 *          triangle A093915.
 * https://oeis.org/A093918
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 + (n+1)/2 (n is odd) a(n) = 6*(n/2)^2 + (n/2) + 1 (n
 *            is even)
 * Range:     u8 n <= 13, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    if (n_seq & 1) {                     /* odd n */
        seq_t k = (n_seq + 1) / 2;       /* k = (n+1)/2 */
        return n_seq * n_seq + k;
    } else {                              /* even n */
        seq_t k = n_seq / 2;             /* k = n/2 */
        return 6 * k * k + k + 1;
    }
}
