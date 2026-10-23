/*
 * A041001  Convolution of A000108(n+1), n >= 0, (Catalan numbers) with
 *          A038845 (3-fold convolution of powers of 4).
 * https://oeis.org/A041001
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n+3)*(3*(n+6)*2^(2*n+3)-(n+4)*binomial(2*n+7, n+3))/12
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t N   = (seq_t)n;
    seq_t pow = pow2((idx_t)(2*n + 3));                           /* 2^(2n+3) */
    seq_t bin = binom((idx_t)(2*n + 7), (idx_t)(n + 3));          /* binomial(2n+7, n+3) */

    seq_t t1 = (N + 6) * pow;                                     /* (n+6)*2^(2n+3) */
    t1 = t1 * 3;                                                   /* times 3 */
    seq_t t2 = (N + 4) * bin;                                     /* (n+4)*binomial(...) */

    seq_t num = (N + 3) * (t1 - t2);

    return num / 12;
}
