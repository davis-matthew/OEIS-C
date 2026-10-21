/*
 * A081672  Expansion of exp(2x) - exp(0) + BesselI_0(2x).
 * https://oeis.org/A081672
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1; a(n)=2^n for odd n; a(n)=2^n + C(n,n/2) for even n>0
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    seq_t pow2n = pow2(n);           /* 2^n */
    if (n & 1)                       /* odd n */
        return pow2n;
    idx_t half = n >> 1;             /* n/2 */
    seq_t comb = binom(n, half);     /* C(n,n/2) */
    return pow2n + comb;
}
