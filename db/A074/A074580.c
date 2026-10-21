/*
 * A074580  a(n) = 7^n + 8^n + 9^n.
 * https://oeis.org/A074580
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 7^n + 8^n + 9^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   power_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/power_of.h"

seq_t a(idx_t n) {
    seq_t p7 = power_of(7, n);
    seq_t p8 = power_of(8, n);
    seq_t p9 = power_of(9, n);
    return p7 + p8 + p9;
}
