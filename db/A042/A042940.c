/*
 * A042940  Convolution of Catalan numbers A000108(n+1), n >= 0, with A038846.
 * https://oeis.org/A042940
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(n+4,2) * ((n+8)*10*4^(n+1) - ( (2*n+9)*C(2*n+8,
 *            n+4) )/2) / 30 where C(m,k) = binomial(m,k). This implementation
 *            directly evaluates the closed form while keeping intermediate
 *            values as small as possible
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 8, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* compute 4^(n+1) */
    seq_t pow4 = 1;
    for (idx_t i = 0; i < n + 1; ++i)
        pow4 *= 4;

    /* compute binomial(2*(n+4), n+4) */
    idx_t k = n + 4;
    seq_t binom = 1;
    for (idx_t i = 1; i <= k; ++i) {
        seq_t num = (seq_t)(k + i);
        binom = (binom * num) / (seq_t)i;
    }

    /* termLeft = 20*(n+8)*4^(n+1) */
    seq_t termLeft = (seq_t)(20 * (n + 8));
    termLeft *= pow4;

    /* termRight = (2*n+9)*binom */
    seq_t termRight = (seq_t)(2 * n + 9);
    termRight *= binom;

    /* difference */
    seq_t diff = termLeft - termRight;

    /* final result = diff * (n+4)*(n+3) / 120 */
    seq_t result = diff * (seq_t)(n + 4);
    result *= (seq_t)(n + 3);
    result /= (seq_t)120;

    return result;
}
