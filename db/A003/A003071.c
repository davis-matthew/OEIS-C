/*
 * A003071  Sorting numbers: maximal number of comparisons for sorting n
 *          elements by list merging.
 * https://oeis.org/A003071
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 1 - 2^e_t + Sum_{k=1..t} (e_k + k - 1)*2^e_k, where n =
 *            2^e_1 + 2^e_2 + ... + 2^e_t, e_1 > e_2 > ... > e_t >= 0
 * Range:     u8 n <= 52, u16 n <= 5631, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow2_helper
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2_helper.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;
    
    // Find bit positions in ascending order (smallest first)
    idx_t e[64];
    int t = 0;
    
    for (int i = 0; i < 64; i++) {
        if ((n >> i) & 1) {
            e[t++] = i;
        }
    }
    
    if (t == 0) return 0;
    
    // a(n) = 1 - 2^e_t + Sum_{k=1..t} (e_k + k - 1)*2^e_k
    // e[0] is e_t (smallest), e[t-1] is e_1 (largest)
    seq_t result = (seq_t)1 - pow2_helper(e[0]);
    
    for (int k = 1; k <= t; k++) {
        idx_t e_k = e[t - k];
        seq_t power2 = pow2_helper(e_k);
        seq_t coeff = (seq_t)(e_k + k - 1);
        result += coeff * power2;
    }
    
    return result;
}
