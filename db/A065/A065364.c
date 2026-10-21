/*
 * A065364  Alternating sum of balanced ternary digits in n. Replace 3^k with
 *          (-1)^k in balanced ternary expansion of n.
 * https://oeis.org/A065364
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, sign
 * Formula:   a(n) = Σ d_k (-1)^k, where n = Σ d_k 3^k and d_k∈{-1,0,1}
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function computes the balanced ternary digits of n and evaluates the
   alternating sum of those digits. */
#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t sign = 1;
    idx_t m = n;
    while (m > 0) {
        unsigned r = m % 3;
        m /= 3;
        seq_t d;
        if (r == 2) {
            d = (seq_t)(-1);
            m += 1;
        } else {
            d = (seq_t)r;   /* r is 0 or 1 */
        }
        sum += d * sign;
        sign = -sign;
    }
    return sum;
}
