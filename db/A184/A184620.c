/*
 * A184620  a(n) = floor(nr+h), where r=sqrt(2), h=1/4; complement of A184621.
 * https://oeis.org/A184620
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*sqrt(2)+1/4)
 * Range:     u8 n <= 5, u16 n <= 90, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Convert index to seq_t for integer arithmetic */
    seq_t ns = (seq_t)n;

    /* Compute 2 * n^2, needed for sqrt and comparison */
    seq_t nsq = ns * ns;      /* n^2   */
    seq_t two_nsq = nsq + nsq;/* 2*n^2 */

    /* floor(n*sqrt(2)) = floor(sqrt(2*n^2)) */
    seq_t a = floor_sqrt(two_nsq);

    /*
     * To decide whether the fractional part of n*sqrt(2)
     * is at least 0.75, compare 32*n^2 with
     * 16*a^2 + 24*a + 9, derived from:
     *   n*sqrt(2) >= a + 0.75
     *   ⇔ (2*n^2) >= (a+0.75)^2
     *   ⇔ 32*n^2 >= 16*a^2 + 24*a + 9
     */
    seq_t left  = two_nsq * 16;          /*  32*n^2 = 16*(2*n^2) */
    seq_t a2    = a * a;
    seq_t right = a2 * 16;               /* 16*a^2 */
    right += a * 24;                     /*  +24*a  */
    right += 9;                          /*  +9     */

    if (left >= right) {
        return a + 1;
    } else {
        return a;
    }
}
