/*
 * A001836  Numbers k such that phi(2k-1) < phi(2k), where phi is Euler's
 *          totient function A000010.
 * https://oeis.org/A001836
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, changed
 * Formula:   a(n) = the n-th number k such that phi(2k-1) < phi(2k)
 * Range:     u8 n <= 3, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   a001226_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a001226_phi.h"

seq_t a(idx_t n) {
    idx_t count = 0;
    uint64_t k = 0;
    
    while (count < n) {
        k++;
        uint64_t phi_2k_minus_1 = a001226_phi(2 * k - 1);
        uint64_t phi_2k = a001226_phi(2 * k);
        
        if (phi_2k_minus_1 < phi_2k) {
            count++;
        }
    }
    
    return (seq_t)k;
}
