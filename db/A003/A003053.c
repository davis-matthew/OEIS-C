/*
 * A003053  Order of orthogonal group O(n, GF(2)).
 * https://oeis.org/A003053
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = product_{k=2}^{n} (2^(k-1) - [k is odd])
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   A001832_pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/A001832_pow2.h"

seq_t a(idx_t n) {
    if (n == 1) return 1;
    
    seq_t result = 1;
    for (idx_t k = 2; k <= n; k++) {
        seq_t power_of_2 = A001832_pow2(k - 1);
        seq_t factor = (k % 2 == 0) ? power_of_2 : (power_of_2 - 1);
        result *= factor;
    }
    
    return result;
}
