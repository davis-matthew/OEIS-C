/*
 * A163120  Sum of the first n digits of the concatenation of all nonprimes.
 * https://oeis.org/A163120
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum of first n digits of the concatenation of all
 *            nonprimes
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    seq_t sum = 0;
    idx_t remaining = n;
    for (uint64_t x = 0; remaining > 0; ++x) {
        if (is_prime((idx_t)x)) continue;
        unsigned char digits[20];
        int len;
        if (x == 0) {
            digits[0] = 0;
            len = 1;
        } else {
            uint64_t temp = x;
            len = 0;
            while (temp > 0) {
                digits[len++] = temp % 10;
                temp /= 10;
            }
            /* reverse digits to get most‑significant‑first order */
            for (int i = 0; i < len / 2; ++i) {
                unsigned char t = digits[i];
                digits[i] = digits[len - 1 - i];
                digits[len - 1 - i] = t;
            }
        }
        for (int i = 0; i < len && remaining > 0; ++i) {
            sum += digits[i];
            --remaining;
        }
    }
    return sum;
}
