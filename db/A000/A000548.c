/*
 * A000548  Squares that are not the sum of 2 nonzero squares.
 * https://oeis.org/A000548
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = m_n^2, where m_n is the n-th nonhypotenuse number
 * Range:     u8 n <= 11, u16 n <= 129, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_nonhypotenuse
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_nonhypotenuse.h"

seq_t a(idx_t n) {
    idx_t count = 0;
    uint64_t m = 0;
    
    while (count < n) {
        m++;
        if (is_nonhypotenuse(m)) {
            count++;
        }
    }
    
    seq_t result = (seq_t)m;
    return result * result;
}
