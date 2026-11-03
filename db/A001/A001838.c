/*
 * A001838  Numbers k such that phi(k+2) = phi(k) + 2.
 * https://oeis.org/A001838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   search for numbers k where phi(k+2) = phi(k) + 2
 * Range:     u8 n <= 29, u16 n <= 1146, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 72 terms through n = 9470 (2026-11-01)
 * Helpers:   totient_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/totient_u64.h"

seq_t a(idx_t n) {
    idx_t count = 0;
    idx_t k = 1;
    
    while (1) {
        if (totient_u64(k + 2) == totient_u64(k) + 2) {
            count++;
            if (count == n) {
                return (seq_t)k;
            }
        }
        k++;
    }
}
