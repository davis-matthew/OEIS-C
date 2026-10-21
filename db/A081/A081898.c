/*
 * A081898  Second binomial transform of binomial(n+4, 4).
 * https://oeis.org/A081898
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n*(n^4 + 42*n^3 + 515*n^2 + 2034*n + 1944)/1944
 * Range:     u8 none, u16 n <= 3, u32 n <= 15, u64 n <= 33, u128 n <= 71,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;

    /* compute powers of n */
    seq_t n2 = n_seq * n_seq;          /* n^2 */
    seq_t n3 = n2 * n_seq;             /* n^3 */
    seq_t n4 = n3 * n_seq;             /* n^4 */

    /* polynomial part: n^4 + 42*n^3 + 515*n^2 + 2034*n + 1944 */
    seq_t poly = n4
               + (seq_t)42 * n3
               + (seq_t)515 * n2
               + (seq_t)2034 * n_seq
               + (seq_t)1944;

    /* 1944 = 8 * 3^5, and the polynomial is always divisible by 8 */
    seq_t poly_div8 = poly / (seq_t)8;  /* exact division */

    if (n >= 5) {
        /* replace 3^n / 3^5 = 3^(n-5) */
        seq_t pow3 = big_pow((seq_t)3, (seq_t)(n - 5));
        return pow3 * poly_div8;
    } else {
        /* for n < 5 perform the division by 3^5 = 243 */
        seq_t pow3 = big_pow((seq_t)3, n_seq);
        seq_t numerator = pow3 * poly_div8;
        return numerator / (seq_t)243;
    }
}
