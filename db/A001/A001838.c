/*
 * A001838  Numbers k such that phi(k+2) = phi(k) + 2.
 * https://oeis.org/A001838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   search for numbers k where phi(k+2) = phi(k) + 2
 * Range:     u8 n <= 29, u16 n <= 1146, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"

seq_t a(idx_t n) {
    idx_t count = 0;
    idx_t k = 1;
    
    while (1) {
        if (phi(k + 2) == phi(k) + 2) {
            count++;
            if (count == n) {
                return (seq_t)k;
            }
        }
        k++;
    }
}
