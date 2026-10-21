/*
 * A228176  Floor(n*(sqrt(10)+sqrt(3))).
 * https://oeis.org/A228176
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(sqrt(10)+sqrt(3)))
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 574, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t nsq = (seq_t)n;
    /* n² and n⁴ */
    seq_t n2   = nsq * nsq;
    seq_t n4   = n2 * n2;

    /* floor(n*sqrt(10)) and floor(n*sqrt(3)) */
    seq_t t1 = floor_sqrt(n2 * 10);
    seq_t t2 = floor_sqrt(n2 * 3);
    seq_t sum = t1 + t2;

    /* Compare (n*sqrt(10)+n*sqrt(3)) with sum+1
       using integer arithmetic:
          120*n⁴ >= (sum² + 2*sum + 1 - 13*n²)²   ⇔ floor = sum+1 */
    seq_t left  = 120 * n4;
    seq_t tmp   = 13 * n2;
    seq_t rhs_inner = (sum * sum + 2 * sum + 1) - tmp;
    seq_t rhs  = rhs_inner * rhs_inner;

    if (left >= rhs) {
        return sum + 1;
    } else {
        return sum;
    }
}
