/*
 * A179524  a(n) = Sum_{k=0..n} (-4)^k*binomial(n,k)^2*binomial(n-k,k)^2.
 * https://oeis.org/A179524
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} (-4)^k * binomial(n,2k)^2 *
 *            binomial(2k,k)^2
 * Range:     i8 n <= 2, i16 n <= 5, i32 n <= 10, i64 n <= 20, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t c_even = 1;      /* C(n, 0) */
    seq_t c_ck   = 1;      /* C(0, 0) */
    seq_t sign   = 1;      /* (-4)^k */

    idx_t kmax = n / 2;

    for (idx_t k = 0; k <= kmax; ++k) {
        /* compute term = sign * c_even^2 * c_ck^2 */
        seq_t t_even  = c_even * c_even;
        seq_t t_ck    = c_ck * c_ck;
        seq_t term    = sign * t_even * t_ck;
        sum += term;

        if (k == kmax) break;   /* no further terms */

        /* Update C(n, 2k) to C(n, 2(k+1)) */
        seq_t a_num = (seq_t)(n - 2 * k);
        seq_t b_num = (seq_t)(n - 2 * k - 1);
        seq_t num   = a_num * b_num;
        seq_t den   = (seq_t)(2 * (k + 1)) * (seq_t)(2 * (k + 1) - 1);
        c_even = c_even * num / den;

        /* Update C(2k, k) to C(2(k+1), k+1) */
        seq_t num2 = (seq_t)(2 * (k + 1)) * (seq_t)(2 * (k + 1) - 1);
        seq_t den2 = (seq_t)(k + 1) * (seq_t)(k + 1);
        c_ck = c_ck * num2 / den2;

        /* Next power of (-4) */
        sign = sign * (seq_t)(-4);
    }

    return sum;
}
