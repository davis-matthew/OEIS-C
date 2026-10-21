/*
 * A057057  Let R(i,j) be the rectangle with antidiagonals 1; 2,3; 4,5,6; ...;
 *          C(n,3) is in antidiagonal a(n).
 * https://oeis.org/A057057
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round( sqrt( 2*binomial(n,3) ) )
 * Range:     u8 n <= 6, u16 n <= 35, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   floor_sqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*           = round( sqrt( n*(n-1)*(n-2) / 3 ) )   */
#include "../utils/floor_sqrt_seq.h"

seq_t a(idx_t n)
{
    /* Offset: 3 – a(3) is the first term. */
    if (n < 3) return 0; /* value not defined for n < 3 */

    seq_t n_seq = (seq_t)n;
    /* numerator of the expression: n*(n-1)*(n-2) */
    seq_t num = n_seq * (n_seq - 1) * (n_seq - 2);

    /* Upper bound for sqrt(num/3) ;
       n*floor(sqrt(n)) is larger than the desired result for all n≥3. */
    seq_t sqrt_n = floor_sqrt_seq(n_seq);
    seq_t high = n_seq * sqrt_n + 1;
    seq_t low  = 0;

    /* Binary search for floor( sqrt(num/3) )  */
    while (low < high) {
        seq_t mid   = (low + high + 1) >> 1;
        seq_t mid_sq3 = mid * mid * 3;   /* mid^2 * 3 */
        if (mid_sq3 <= num)
            low = mid;
        else
            high = mid - 1;
    }
    seq_t k = low;   /* floor part */

    /* Rounding step: compare 4*num with 12*k^2 + 12*k + 3
       (derived from the condition sqrt(num/3) <= k+0.5) */
    seq_t lhs = num * 4;
    seq_t rhs = (k * k) * 12 + k * 12 + 3;

    return (lhs <= rhs) ? k : k + 1;
}
