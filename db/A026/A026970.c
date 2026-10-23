/*
 * A026970  a(n) = Sum_{k=0..n} (k+1) * A026637(n,k).
 * https://oeis.org/A026970
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = -((-1)^n-2^(n+2)+3)*(n+2)/6 for n>0 a(0) = 1 The
 *            calculation avoids negative intermediate values by rewriting the
 *            expression as a(n) = ( (-1)^(n+1) + 2^(n+2) - 3 )*(n+2)/6 which
 *            is computed using only unsigned arithmetic and the provided big
 *            integer power function
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 25, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"  // for big_pow()

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* Compute 2^(n+2) as a big integer */
    seq_t pow2 = big_pow((seq_t)2, (seq_t)(n + 2));

    /* Subtract 3 */
    seq_t val = pow2 - (seq_t)3;

    /* Add (-1)^(n+1) which is +1 when n is odd, -1 when n is even */
    if ((n & (idx_t)1) == 0) {       /* n even ⇒ add (-1) */
        val = val - (seq_t)1;
    } else {
        val = val + (seq_t)1;
    }

    /* Multiply by (n+2) */
    val = val * ((seq_t)n + (seq_t)2);

    /* Divide by 6 to finish the formula */
    val = val / (seq_t)6;

    return val;
}
