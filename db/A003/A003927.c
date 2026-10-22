/*
 * A003927  Order of simple Chevalley group B_n (3).
 * https://oeis.org/A003927
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3^(n^2) * Product_{k=1..n}(3^(2*k)-1) / 2
 * Range:     u8 none, u16 n <= 2, u32 n <= 2, u64 n <= 4, u128 n <= 6,
 *            big any n
 * Helpers:   a000272_ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a000272_ipow.h"

seq_t a(idx_t n) {
    seq_t result = a000272_ipow((seq_t)3, n * n);
    seq_t power_3_2k = 9;  /* 3^2 */
    
    for (idx_t k = 1; k <= n; k++) {
        result = result * (power_3_2k - 1);
        power_3_2k = power_3_2k * 9;
    }
    
    result = result / 2;
    return result;
}
