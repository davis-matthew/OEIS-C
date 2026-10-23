/*
 * A100343  Denominators of the convergents in the continued fraction
 *          expansion for twice the constant given by A100338, where the
 *          partial quotients equal A006519 (greatest power of 2 dividing n)
 *          interleaved with 2's.
 * https://oeis.org/A100343
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  cofr, nonn
 * Formula:   a(1) = 1, a(2) = 1 a(2n-1) = 2*a(2n-2) + a(2n-3) for n>1 a(2n) =
 *            a(2n-1)*A006519(n) + a(2n-2) for n>1 (A006519(n) = greatest
 *            power of 2 dividing n)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 27, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 2)
        return (seq_t)1;

    seq_t a_prev2 = (seq_t)1;   /* a(1)  */
    seq_t a_prev1 = (seq_t)1;   /* a(2)  */
    seq_t a_curr  = (seq_t)0;   /* placeholder */

    for (idx_t i = 3; i <= n; ++i) {
        if (i & 1) { /* odd: a(2n-1) = 2*a_prev1 + a_prev2 */
            a_curr = (seq_t)2 * a_prev1 + a_prev2;
        } else {    /* even: a(2n) = a_prev1*A006519(i/2) + a_prev2 */
            idx_t k = i >> 1;
            int shift = 0;
            idx_t t = k;
            while ((t & 1) == 0) {
                ++shift;
                t >>= 1;
            }
            seq_t pow = (seq_t)1;
            for (int s = 0; s < shift; ++s)
                pow <<= 1;
            a_curr = a_prev1 * pow + a_prev2;
        }
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }

    return a_curr;
}
