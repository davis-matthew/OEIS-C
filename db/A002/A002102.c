/*
 * A002102  Number of nonnegative solutions to x^2 + y^2 + z^2 = n.
 * https://oeis.org/A002102
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Coefficient of q^n in (1 + q + q^4 + q^9 + ...)^3, i.e., number
 *            of nonnegative solutions to x^2 + y^2 + z^2 = n
 * Range:     u8 n <= 493, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isqrt_helper
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/isqrt_helper.h"

seq_t a(idx_t n)
{
    seq_t count = 0;
    seq_t n_seq = (seq_t)n;
    
    for (seq_t x = 0; x * x <= n_seq; x++) {
        seq_t x_sq = x * x;
        seq_t remainder_x = n_seq - x_sq;
        
        for (seq_t y = 0; y * y <= remainder_x; y++) {
            seq_t y_sq = y * y;
            seq_t remainder_y = remainder_x - y_sq;
            
            seq_t z = isqrt_helper(remainder_y);
            if (z * z == remainder_y) {
                count++;
            }
        }
    }
    
    return count;
}
