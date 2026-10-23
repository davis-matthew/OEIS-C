/*
 * A041000  If decimal expansion of n-th prime is x1 x2 ... xk, sort the xi
 *          into nonincreasing order, y1 y2 ... yk; then a(n) =
 *          |y1-y2-y3...-yk|.
 * https://oeis.org/A041000
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = |2*max_digit(n-th prime) - sum_of_digits(n-th prime)|
 * Range:     u8 n <= 106, u16 n <= 106, u32 n <= 106, u64 n <= 106,
 *            u128 n <= 106, big any n
 * Helpers:   abs_int, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/abs_int.h"

seq_t a(idx_t n)
{
    /* get the n-th prime (1‑based) */
    uint32_t p = get_prime(n);

    /* find the maximum digit and the sum of all digits */
    int max_digit = 0;
    int sum_digits = 0;
    uint32_t tmp = p;
    while (tmp) {
        int d = tmp % 10;
        sum_digits += d;
        if (d > max_digit) {
            max_digit = d;
        }
        tmp /= 10;
    }

    /* compute the absolute difference */
    int diff = (2 * max_digit) - sum_digits;
    return (seq_t)abs_int(diff);
}
