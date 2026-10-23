/*
 * A188192  a(n) = [n*r]-[k*r]-[n*r-k*r], where r=sqrt(5), k=3, [ ]=floor.
 * https://oeis.org/A188192
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*sqrt(5)) - floor(3*sqrt(5)) -
 *            floor((n-3)*sqrt(5))
 * Range:     u8 n <= 3, u16 n <= 57, u32 n <= 134, u64 n <= 134,
 *            u128 n <= 134, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    const int64_t floor_k = 6;                 /* floor(3*sqrt(5)) */

    /* convert n to a signed type for safe subtraction */
    int64_t ns = (int64_t)n;

    /* floor(n*sqrt(5))  = floor(sqrt(5*n^2)) */
    seq_t t_n = (seq_t)5 * (seq_t)ns * (seq_t)ns;
    int64_t floor_n = (int64_t)floor_sqrt(t_n);

    /* floor((n-3)*sqrt(5)) */
    int64_t floor_m;
    if (ns >= 3) {
        int64_t m = ns - 3;
        seq_t t_m = (seq_t)5 * (seq_t)m * (seq_t)m;
        floor_m = (int64_t)floor_sqrt(t_m);
    } else {
        /* n < 3 : (n-3) is negative */
        int64_t abs_m = 3 - ns;                /* positive value of |n-3| */
        seq_t t_abs = (seq_t)5 * (seq_t)abs_m * (seq_t)abs_m;
        int64_t floor_abs = (int64_t)floor_sqrt(t_abs);  /* floor(|n-3|*sqrt(5)) */
        floor_m = -(floor_abs + 1);           /* floor(-(n-3)*sqrt(5)) */
    }

    /* assemble the result; the value is always 0 or 1 */
    int64_t result = floor_n - floor_k - floor_m;
    return (seq_t)result;
}
