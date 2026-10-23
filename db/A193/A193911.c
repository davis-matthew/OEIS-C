/*
 * A193911  Sums of the diagonals of the matrix formed by listing the h-Stohr
 *          sequences in increasing order.
 * https://oeis.org/A193911
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   For odd n ≥ 1: a(n) = 2^((n+1)/2) - n + ((n+1)/2) - 2 +
 *            Σ_{i=0}^{((n+1)/2)-1} (2i+1) · ( 2^(((n+1)/2)-i) - 1 ) For even
 *            n ≥ 2: a(n) = 2^((n/2)+2) - n - 4 + Σ_{i=0}^{(n/2)-1} (2i+1) · (
 *            2^((n/2)-i) - 1 )
 * Range:     u8 n <= 10, u16 n <= 25, u32 n <= 57, u64 n <= 121,
 *            u128 n <= 249, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/big_pow.h"   /* provides seq_t big_pow(seq_t base, idx_t exp) */

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* offset starts at 1, behaviour below not used */

    seq_t res = 0;
    idx_t k;
    seq_t pow;

    if ((n & 1U) != 0)   /* odd n */
    {
        k = (n + 1) >> 1;                     /* (n+1)/2 */
        pow = big_pow((seq_t)2, k);          /* 2^k */
        res = pow - (seq_t)n + (seq_t)k - 2;

        seq_t curPow = pow;
        for (idx_t i = 0; i < k; ++i)
        {
            seq_t term = ((seq_t)(2 * i + 1)) * (curPow - 1);
            res += term;
            curPow >>= 1;                     /* next power 2^(k-i-1) */
        }
    }
    else   /* even n */
    {
        k = n >> 1;                            /* n/2 */
        pow = big_pow((seq_t)2, k + 2);        /* 2^(k+2) */
        res = pow - (seq_t)n - 4;

        seq_t curPow = big_pow((seq_t)2, k);   /* 2^k */
        for (idx_t i = 0; i < k; ++i)
        {
            seq_t term = ((seq_t)(2 * i + 1)) * (curPow - 1);
            res += term;
            curPow >>= 1;
        }
    }
    return res;
}
