/*
 * A007417  If k appears, 3k does not.
 * https://oeis.org/A007417
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   search: a(n) = n-th positive integer k whose 3-adic valuation
 *            (number of trailing ternary zeros) is even
 * Range:     u8 n <= 191, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

static bool is_in_sequence(seq_t k) {
    idx_t trailing_zeros = 0;
    while (k % 3 == 0) {
        trailing_zeros++;
        k /= 3;
    }
    return trailing_zeros % 2 == 0;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    seq_t k = 1;
    while (1) {
        if (is_in_sequence(k)) {
            count++;
            if (count == n) {
                return k;
            }
        }
        k++;
    }
}
