/*
 * A152291  a(n) = (n+1)^floor((n-1)/2).
 * https://oeis.org/A152291
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1)^floor((n-1)/2)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 16, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   big_pow, seq_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/seq_one.h"

seq_t a(idx_t n)
{
    /* Handle n = 0 separately: floor((-1)/2) = -1, but 1^(-1) = 1 */
    if (n == 0)
        return seq_one();

    /* Compute e = floor((n-1)/2) for n >= 1 :
       e = n/2 for odd n,  e = n/2 - 1 for even n */
    idx_t e = n / 2;
    if ((n & 1ULL) == 0)
        e -= 1;

    /* Compute (n+1)^e */
    return big_pow((seq_t)(n + 1), (seq_t)e);
}
