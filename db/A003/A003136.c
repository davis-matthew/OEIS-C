/*
 * A003136  Loeschian numbers: numbers of the form x^2 + xy + y^2; norms of
 *          vectors in A2 lattice.
 * https://oeis.org/A003136
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  core, easy, nonn, nice
 * Formula:   n is in the sequence iff for each prime p ≡ 2 (mod 3) dividing
 *            n, the exponent is even
 * Range:     u8 n <= 82, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_loeschian
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_loeschian.h"

seq_t a(idx_t n) {
    idx_t count = 0;
    seq_t val = 0;
    
    while (1) {
        if (is_loeschian(val)) {
            count++;
            if (count == n) {
                return val;
            }
        }
        val++;
    }
}
