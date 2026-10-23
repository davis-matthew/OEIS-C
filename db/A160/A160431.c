/*
 * A160431  Numerator of Hermite(n, 19/32).
 * https://oeis.org/A160431
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = numerator of H_n(19/32) where H_n is the physicist
 *            Hermite polynomial. Recurrence: H_0 = 1, H_1 = 2x, H_{n+1} = 2x
 *            H_n - 2n H_{n-1} with x = 19/32. We keep the fraction as
 *            (numerator, 2^shift) and reduce common powers of two after each
 *            step. Only integers and bit shifts are used
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 6, i64 n <= 11, i128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)19;

    seq_t prev2_num = (seq_t)1;    /* H_0 */
    seq_t prev1_num = (seq_t)19;   /* H_1 */
    unsigned long long prev2_shift = 0; /* 2^0 */
    unsigned long long prev1_shift = 4; /* denominator 2^4 = 16 */

    seq_t curr_num;
    unsigned long long curr_shift;

    for (idx_t i = 2; i <= n; ++i) {
        /* term1 = (19 * prev1_num) / 2^(prev1_shift + 4)   */
        seq_t term1_num = (seq_t)19 * prev1_num;
        unsigned long long d1 = prev1_shift + 4;

        /* term2 = (2*(i-1) * prev2_num) / 2^prev2_shift   */
        seq_t term2_num = (seq_t)2 * ((seq_t)(i - 1)) * prev2_num;
        unsigned long long d2 = prev2_shift;

        /* combine with common denominator 2^d1 (d1 >= d2) */
        curr_num = term1_num - (term2_num << (d1 - d2));
        curr_shift = d1;

        /* reduce common power of two: divide by 2^t where t <= curr_shift */
        if (curr_num == 0) {
            curr_shift = 0; /* 0/1 */
        } else {
            seq_t abs_num = curr_num < 0 ? -curr_num : curr_num;
            unsigned long long t = 0;
            while (t < curr_shift && ((abs_num & 1) == 0)) {
                abs_num >>= 1;
                ++t;
            }
            curr_num >>= t;
            curr_shift -= t;
        }

        /* shift to next step */
        prev2_num = prev1_num;   prev2_shift = prev1_shift;
        prev1_num = curr_num;    prev1_shift = curr_shift;
    }
    return prev1_num;
}
