/*
 * A002479  Numbers of the form x^2 + 2*y^2.
 * https://oeis.org/A002479
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   Numbers k such that every prime p ≡ 5, 7 (mod 8) dividing k has
 *            even exponent
 * Range:     u8 n <= 111, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_in_sequence
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_in_sequence.h"

seq_t a(idx_t n) {
    idx_t count = 0;
    seq_t m = 0;
    
    while (true) {
        if (is_in_sequence(m)) {
            count++;
            if (count == n) {
                return m;
            }
        }
        m++;
    }
}
