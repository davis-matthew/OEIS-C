/*
 * A179537  a(n) = Sum_{k=0..n} binomial(n,k)^2*binomial(n-k,k)^2*(-16)^k.
 * https://oeis.org/A179537
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} binomial(n,k)^2 * binomial(n-k,k)^2
 *            * (-16)^k
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 8, i64 n <= 16, i128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    idx_t kmax = n / 2;          /* binomial(n-k, k) is zero for k > n/2 */
    seq_t sum = 0;
    seq_t cn   = 1;              /* C(n,0) */
    seq_t pow_neg16 = 1;         /* (-16)^0 */

    for (idx_t k = 0; k <= kmax; ++k) {
        /* Compute binomial(n-k, k) */
        seq_t bn = 1;            /* C(n-k,0) = 1 for k=0 */
        for (idx_t j = 1; j <= k; ++j) {
            seq_t num = (seq_t)(n - k - j + 1);   /* numerator term */
            bn = bn * num / (seq_t)j;             /* divide exactly */
        }

        /* term = cn^2 * bn^2 * (-16)^k */
        seq_t term = cn * cn;
        term = term * bn * bn;
        term = term * pow_neg16;

        sum += term;

        /* Prepare for next k */
        if (k < kmax) {
            /* Update C(n,k+1) = C(n,k) * (n-k)/(k+1) */
            seq_t numerator   = (seq_t)(n - k);
            seq_t denominator = (seq_t)(k + 1);
            cn = cn * numerator / denominator;

            /* Update power (-16)^(k+1) = (-16)^k * (-16) */
            pow_neg16 = pow_neg16 * (seq_t)-16;
        }
    }

    return sum;
}
