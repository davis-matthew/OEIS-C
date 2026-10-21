/*
 * A099979  Bisection of A001157: sigma_2(2n).
 * https://oeis.org/A099979
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{d | 2n} d^2 (sigma_2 of 2n)
 * Range:     u8 n <= 7, u16 n <= 107, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Compute sigma_2(2*n) by iterating over all divisors */
    seq_t m = (seq_t)n * (seq_t)2;          /* m = 2*n  */
    seq_t sum = 0;
    for (idx_t d = 1; ; ++d) {
        seq_t dd = (seq_t)d * (seq_t)d;    /* d*d, safe because d <= sqrt(m) */
        if (dd > m) break;                /* exceeded sqrt(m)  */
        if (m % (seq_t)d == 0) {          /* d divides m      */
            seq_t other = m / (seq_t)d;
            if (other != d) {
                sum += dd + other * other;
            } else {
                sum += dd;                /* perfect square   */
            }
        }
    }
    return sum;
}
