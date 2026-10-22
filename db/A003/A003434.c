/*
 * A003434  Number of iterations of phi(x) at n needed to reach 1.
 * https://oeis.org/A003434
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice, look
 * Formula:   a(n) = a(phi(n)) + 1 for n >= 2, a(1) = 0; compute by
 *            iteratively applying phi until reaching 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   compute_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/compute_phi.h"

seq_t a(idx_t n) {
    seq_t count = 0;
    uint64_t current = (uint64_t)n;
    
    while (current > 1) {
        current = compute_phi(current);
        count++;
    }
    
    return count;
}
