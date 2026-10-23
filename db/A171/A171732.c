/*
 * A171732  a(n) = Sum_{d|n} d*2^(d^2).
 * https://oeis.org/A171732
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} d*2^(d^2)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for n=0, return 0 for safety */
    seq_t sum = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            idx_t d2 = d * d;
            seq_t pow = seq_pow((seq_t)2, d2);
            sum += (seq_t)d * pow;

            if (d != n / d) {
                idx_t d3 = n / d;
                idx_t d32 = d3 * d3;
                seq_t pow3 = seq_pow((seq_t)2, d32);
                sum += (seq_t)d3 * pow3;
            }
        }
    }
    return sum;
}
