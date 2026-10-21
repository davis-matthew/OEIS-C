/*
 * A089343  Sum of all digits in all even numbers from 0 to 6(10^(k+1)-1)/9
 *          (with (k+1) 6's).
 * https://oeis.org/A089343
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n*(8+3*10^n)/2 - 7*(10^n-1)/9
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* Compute 10^n */
    seq_t ten_n = pow10(n);

    /* Positive part: n * (8 + 3*10^n) / 2 */
    seq_t pos = (seq_t)n * ((seq_t)8 + (seq_t)3 * ten_n);
    pos = pos / 2;

    /* Negative part: 7 * (10^n - 1) / 9 */
    seq_t neg = (seq_t)7 * (ten_n - (seq_t)1);
    neg = neg / 9;

    /* a(n) = pos - neg */
    return pos - neg;
}
