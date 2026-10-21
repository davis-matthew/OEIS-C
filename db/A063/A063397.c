/*
 * A063397  T(4,n) with T(n,m) as in A063394.
 * https://oeis.org/A063397
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (6*(-4+27*2^n)+191*2^n*n+15*2^(1+n)*n^2+2^n*n^3)/24 for
 *            n>0 a(0) = 1
 * Range:     u8 n <= 0, u16 n <= 8, u32 n <= 21, u64 n <= 50, u128 n <= 110,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Compute the inner polynomial: 162 + 191*n + 30*n^2 + n^3   */
    seq_t nn = (seq_t)n;
    seq_t inner = (seq_t)162
                + (seq_t)191 * nn
                + (seq_t)30  * nn * nn
                +           nn * nn * nn;

    /* Compute 2^n as a seq_t */
    seq_t p = pow2(n);

    /* Multiply p by inner, avoiding overflow when seq_t is a built‑in type */
    seq_t result;
    /* Use 128‑bit intermediate for built‑in types up to 64 bits */
    if (sizeof(seq_t) <= sizeof(unsigned long long)) {
        unsigned __int128 prod = (unsigned __int128)p * (unsigned __int128)inner;
        prod -= 24;               /* subtract constant */
        prod /= 24;               /* divide by 24 */
        result = (seq_t)prod;
    } else {
        seq_t prod = p * inner;   /* arbitrary‑precision multiplication */
        seq_t temp = prod - (seq_t)24;
        result = temp / (seq_t)24;
    }

    return result;
}
