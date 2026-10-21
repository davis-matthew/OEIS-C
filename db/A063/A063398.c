/*
 * A063398  T(5,n) with T(n,m) as in A063394.
 * https://oeis.org/A063398
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) =
 *            (24*(-8+81*2^n)+1671*2^(1+n)*n+803*2^n*n^2+27*2^(1+n)*n^3+2^n*n^4)
 *            / 192 for n > 0, a(0) = 1
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 19, u64 n <= 47, u128 n <= 107,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    /* a(0) */
    if (n == 0) {
        return (seq_t)1;
    }

    /* Common powers and intermediates */
    seq_t nn  = (seq_t)n;
    /* Q(n) = 1944 + 3342*n + 803*n^2 + 54*n^3 + n^4 */
    seq_t n2  = nn * nn;               /* n^2 */
    seq_t n3  = n2 * nn;               /* n^3 */
    seq_t n4  = n2 * n2;               /* n^4 */
    seq_t Q   = (seq_t)1944
                + (seq_t)3342 * nn
                + (seq_t)803  * n2
                + (seq_t)54   * n3
                + n4;

    /* For n >= 6 we can use the simplified form:
          a(n) = -1 + 2^(n-6) * (Q / 3)
    */
    if (n >= 6) {
        /* Q is always divisible by 3 */
        seq_t Qdiv3 = Q / (seq_t)3;
        seq_t pow2k = pow_seq((seq_t)2, (int)(n - 6)); /* 2^(n-6) */
        seq_t prod   = pow2k * Qdiv3;                  /* exact multiplication */
        return prod - (seq_t)1;
    }

    /* For n = 1..5 the general expression is safe from overflow */
    seq_t pow2n   = pow_seq((seq_t)2, (int)n);        /* 2^n  */
    seq_t numerator = pow2n * Q - (seq_t)192;         /* = 2^n*Q - 192  */
    seq_t result    = numerator / (seq_t)192;         /* exact division */
    return result;
}
