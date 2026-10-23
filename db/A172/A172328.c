/*
 * A172328  a(n) = floor(n*(sqrt(11)+sqrt(3))).
 * https://oeis.org/A172328
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = floor(n*(sqrt(11)+sqrt(3)))
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 623, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Integer square root: floor(sqrt(x)) for unsigned seq_t */
static seq_t floor_sqrt(seq_t x)
{
    if (x < 2) return x;
    seq_t low = 0;
    seq_t high = 1;

    /* Find an upper bound by doubling until high^2 > x */
    while (high * high <= x) high <<= 1;

    low = high >> 1;

    /* Binary search between low and high for the floor sqrt */
    while (low < high) {
        seq_t mid = (low + high + 1) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x) {
            low = mid;
        } else {
            high = mid - 1;
        }
    }
    return low;
}

seq_t a(idx_t n)
{
    /* cast idx_t to seq_t for arithmetic */
    seq_t ns = (seq_t)n;
    seq_t ns_sq = ns * ns;                     /* n^2 */

    /* floor(sqrt(11*n^2)) and floor(sqrt(3*n^2)) */
    seq_t s1 = floor_sqrt( (seq_t)11 * ns_sq );
    seq_t s2 = floor_sqrt( (seq_t)3  * ns_sq );

    seq_t sum = s1 + s2;

    /* Compute (sum+1)^2 and 14*n^2 once */
    seq_t sum_plus_1 = sum + 1;
    seq_t lhs1 = sum_plus_1 * sum_plus_1;                 /* (sum+1)^2 */
    seq_t rhs14 = (seq_t)14 * ns_sq;                      /* 14*n^2 */

    seq_t carry;
    if (lhs1 <= rhs14) {
        /* (sum+1)^2 <= 14*n^2  => fractional parts sum >= 1 */
        carry = 1;
    } else {
        /* rhs2 = (sum+1)^2 - 14*n^2  (positive) */
        seq_t rhs2 = lhs1 - rhs14;

        /* Compare rhs2^2 <= 4*n^4*33 */
        seq_t lhs = rhs2 * rhs2;                /* rhs2^2 */
        seq_t ns_sq_sq = ns_sq * ns_sq;        /* n^4 */
        seq_t rhs = (seq_t)4 * ns_sq_sq * 33;  /* 4*n^4*33 */

        carry = (lhs <= rhs) ? 1 : 0;
    }

    return sum + carry;
}
