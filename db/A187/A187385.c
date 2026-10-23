/*
 * A187385  a(n) = floor(r*n), where r=1+sqrt(3)+sqrt(2); complement of
 *          A187386.
 * https://oeis.org/A187385
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((1 + sqrt(3) + sqrt(2)) * n)
 * Range:     u8 n <= 4, u16 n <= 65, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Nothing special for n == 0 – the offset of the sequence is 1 */
    if (n == 0)
        return (seq_t)0;

    /* n² as a larger width type to avoid overflow for the intermediate
       calculations (idx_t is uint64_t). */
    idx_t nsq = (idx_t)n * (idx_t)n;

    /* floor(sqrt(3*n²)) and floor(sqrt(2*n²)) */
    seq_t A = floor_sqrt((seq_t)(3 * nsq));
    seq_t B = floor_sqrt((seq_t)(2 * nsq));

    /* sum of the two floor terms   */
    seq_t sum_floor = A + B;

    /* S = sum_floor + 1  (used in the fractional‑part test) */
    seq_t S = sum_floor + (seq_t)1;

    /* 5*n² and (sum_floor+1)²   */
    idx_t rhs_limit = 5 * nsq;
    idx_t rhs_input = (idx_t)S * (idx_t)S;

    /* Determine whether the fractional parts of the two square roots add
       to at least 1.  The condition is equivalent to
           24 * n⁴  ≥  R²
       where R = rhs_input - rhs_limit.                 */
    bool add_one = false;
    if (rhs_input <= rhs_limit) {
        add_one = true;
    } else {
        idx_t R = rhs_input - rhs_limit;
        /* Use 128‑bit integer for the comparison to avoid overflow.   */
        unsigned __int128 lhs = (unsigned __int128)24 * nsq * nsq;
        unsigned __int128 rhs = (unsigned __int128)R * R;
        if (lhs >= rhs)
            add_one = true;
    }

    /* Final result: n + floor(sqrt(3*n²)) + floor(sqrt(2*n²)) [+ 1 if needed].
       The cast back to seq_t is safe because seq_t is chosen large enough
       for the final value.                                           */
    seq_t result = (seq_t)n + sum_floor + (add_one ? (seq_t)1 : (seq_t)0);
    return result;
}
