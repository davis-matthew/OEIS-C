/*
 * A093849  Row sums of triangle A093846.
 * https://oeis.org/A093849
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = n*(2*10^(n-1) - 2 + (n+1)*floor(9*10^(n-1)/n))/2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset is 1, guard index 0 */
    seq_t p   = pow10(n - 1);         /* 10^(n-1)  */
    seq_t div = (seq_t)9 * p / n;     /* floor(9*10^(n-1)/n) */
    seq_t temp = (seq_t)2 * p - 2 + (seq_t)(n + 1) * div;
    if (n & 1) {                     /* n odd: temp is even */
        seq_t half = temp / 2;
        return (seq_t)n * half;
    } else {                         /* n even: n/2 is integer */
        return ((seq_t)(n / 2)) * temp;
    }
}
