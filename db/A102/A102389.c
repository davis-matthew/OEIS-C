/*
 * A102389  An evil count.
 * https://oeis.org/A102389
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = (1+floor(n/2)) * (1+(-1)^A000120(n))/2 which simplifies
 *            to a(n) = (1 + n/2) if n is an evil number (binary popcount
 *            even), otherwise a(n) = 0
 * Range:     u8 n <= 509, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_evil
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/is_evil.h"

seq_t a(idx_t n) {
    if (is_evil(n)) {
        /* (n >> 1) + 1 works for unsigned arithmetic */
        return ((seq_t)(n >> 1)) + (seq_t)1;
    } else {
        return 0;
    }
}
