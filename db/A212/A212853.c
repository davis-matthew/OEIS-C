/*
 * A212853  Number of n X 6 arrays with rows being permutations of 0..5 and no
 *          column j greater than column j-1 in all rows.
 * https://oeis.org/A212853
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = -1 + 2*6^n + 2*15^n + 20^n - 3*30^n - 6*60^n - 90^n +
 *            4*120^n + 6*180^n - 5*360^n + 720^n (n ≥ 1)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 13,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* silently handle offset < 1 */

    /* Compute all required powers once */
    seq_t p6   = big_pow((seq_t)6,   (seq_t)n);
    seq_t p15  = big_pow((seq_t)15,  (seq_t)n);
    seq_t p20  = big_pow((seq_t)20,  (seq_t)n);
    seq_t p30  = big_pow((seq_t)30,  (seq_t)n);
    seq_t p60  = big_pow((seq_t)60,  (seq_t)n);
    seq_t p90  = big_pow((seq_t)90,  (seq_t)n);
    seq_t p120 = big_pow((seq_t)120, (seq_t)n);
    seq_t p180 = big_pow((seq_t)180, (seq_t)n);
    seq_t p360 = big_pow((seq_t)360, (seq_t)n);
    seq_t p720 = big_pow((seq_t)720, (seq_t)n);

    /* Sum positive and negative contributions separately */
    seq_t sum_pos = 0, sum_neg = 0;

    /* positive terms */
    sum_pos += p6   * 2;
    sum_pos += p15  * 2;
    sum_pos += p20;
    sum_pos += p120 * 4;
    sum_pos += p180 * 6;
    sum_pos += p720;

    /* negative terms */
    sum_neg += p30  * 3;
    sum_neg += p60  * 6;
    sum_neg += p90;
    sum_neg += p360 * 5;

    /* Combine them: result = sum_pos - sum_neg - 1 */
    seq_t result = sum_pos;
    result -= sum_neg;
    result -= (seq_t)1;

    return result;
}
