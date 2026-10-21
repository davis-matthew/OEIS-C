/*
 * A062834  Denominator of sum of first n terms of the series 1/3 + 1/8 + 1/24
 *          ... in which the denominators are one less than a perfect square
 *          that cannot otherwise be written as a power (cf. A062757,
 *          A037450).
 * https://oeis.org/A062834
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = denominator of the sum of the first n terms of the series
 *            Σ 1/(k^2 – 1) where k is not a perfect power
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 17, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   gcd, is_perfect_power_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/is_perfect_power_uint64.h"

seq_t a(idx_t n) {
    seq_t sum_num = 0;
    seq_t sum_den = 1;
    idx_t k = 2;
    idx_t terms = 0;

    while (terms < n) {
        if (!is_perfect_power_uint64(k)) {
            seq_t d = (seq_t)k * k - 1;            /* k^2 - 1 */
            seq_t new_num = sum_num * d + sum_den;
            seq_t new_den = sum_den * d;
            seq_t g = gcd(new_num, new_den);
            sum_num = new_num / g;
            sum_den = new_den / g;
            ++terms;
        }
        ++k;
    }

    return sum_den;
}
