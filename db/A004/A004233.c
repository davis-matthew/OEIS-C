/*
 * A004233  a(n) = ceiling(log(n)).
 * https://oeis.org/A004233
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceiling(log(n)), i.e. the least k >= 0 with n <= e^k
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

seq_t a(idx_t n) {
    long long k = 0;
    long double x = (long double)n;
    /* smallest k with e^k >= n; e^k is never an integer for k >= 1 */
    while (expl((long double)k) < x)
        k++;
    return (seq_t)k;
}
