/*
 * A112282  a(n) = (-1)^n*(2*n+1) (mod 9).
 * https://oeis.org/A112282
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (-1)^n*(2*n+1) mod 9
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    idx_t n_mod9 = n % 9;
    seq_t term = (n_mod9 * 2 + 1) % 9;
    if (n & 1) {
        if (term == 0)
            return 0;
        return 9 - term;
    }
    return term;
}
