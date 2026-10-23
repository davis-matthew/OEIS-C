/*
 * A015319  Gaussian binomial coefficient [ n,5 ] for q = -12.
 * https://oeis.org/A015319
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Gaussian binomial coefficient [n,5] for q = -12
 * Range:     i8 n <= 5, i16 n <= 5, i32 n <= 6, i64 n <= 8, i128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>  /* for size_t */

seq_t a(idx_t n)
{
    /* For indices below the offset (5) we do not need correct values. */
    if (n < 5) return (seq_t)0;

    /* constants */
    const seq_t Q = (seq_t)(-12);

    /* powers of Q: pow[i] = Q^(n-5+i) for the current n.
       Initially for n = 5 we have
           pow[0] = Q^0  =  1
           pow[1] = Q^1  = -12
           pow[2] = Q^2  = 144
           pow[3] = Q^3  = -1728
           pow[4] = Q^4  = 20736
    */
    seq_t pow[5];
    pow[0] = (seq_t)1;
    for (int i = 1; i < 5; ++i) pow[i] = pow[i - 1] * Q;

    /* B_prev holds binom_q(n-1, k) for k = 0..5.
       For n = 5 we need the values for n-1 = 4, which are:
           [1,  -1595, 19285, -1595, 1, 0]
    */
    seq_t B_prev[6] = {
        (seq_t)1,          /* k = 0 */
        (seq_t)-1595,      /* k = 1 */
        (seq_t)19285,      /* k = 2 */
        (seq_t)-1595,      /* k = 3 */
        (seq_t)1,          /* k = 4 */
        (seq_t)0           /* k = 5 */
    };

    seq_t B[6];

    /* Iterate from n = 5 up to the requested n */
    for (idx_t cur = 5; cur <= n; ++cur) {
        B[0] = (seq_t)1;   /* binom_q(cur, 0) = 1 */
        for (int k = 1; k <= 5; ++k) {
            /* B[cur][k] = B_prev[k] + Q^(cur-k) * B_prev[k-1] */
            seq_t term = B_prev[k];
            /* power index: 5-k corresponds to exponent cur-k */
            term += pow[5 - k] * B_prev[k - 1];
            B[k] = term;
        }

        if (cur == n) return B[5];

        /* Prepare powers for the next n (+1): each exponent increases by 1 */
        for (int i = 0; i < 5; ++i) pow[i] *= Q;

        /* copy current B into B_prev for next iteration */
        for (int k = 0; k < 6; ++k) B_prev[k] = B[k];
    }

    /* Should never reach here but keep compiler happy */
    return B[5];
}
