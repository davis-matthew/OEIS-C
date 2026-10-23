/*
 * A184627  floor((n-h)*s+h), where s=(3+sqrt(3))/2 and h=1/4; complement of
 *          A184626.
 * https://oeis.org/A184627
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((n-1/4)*(3+sqrt(3))/2 + 1/4)
 * Range:     u8 n <= 1, u16 n <= 18, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1.  For n == 0 we return 0
       (value is irrelevant because the OEIS offset is 1). */
    if (n == 0) return 0;

    /* k = 4*n - 1  (integer)
       The term is floor( (12*n - 1 + floor(k*sqrt(3)) ) / 8 ). */
    seq_t k   = (seq_t)(4 * n - 1);
    seq_t val = k * k;           /* k²  */
    val = val * 3;               /* k²·3  */
    seq_t sqrt_term = floor_sqrt(val);   /* floor(k*sqrt(3)) */

    seq_t x = (seq_t)(12 * n - 1) + sqrt_term;   /* 12n-1 + floor(k*sqrt(3)) */
    return x / 8;                                 /* floor division by 8 */
}
