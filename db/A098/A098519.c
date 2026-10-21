/*
 * A098519  E.g.f. exp(x)*BesselI(1,2*sqrt(3)*x)/sqrt(3).
 * https://oeis.org/A098519
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} binomial(n,k)*binomial(n-k,k+1)*3^k
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 31, u128 n <= 61,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;
    seq_t sum = 0;
    seq_t pow3 = 1;          /* 3^k for current k */
    idx_t half = n >> 1;     /* floor(n/2) */
    for (idx_t k = 0; k <= half; ++k) {
        seq_t t1 = binomial(n, k);
        seq_t t2 = binomial(n - k, k + 1);
        seq_t term = t1 * t2 * pow3;
        sum += term;
        pow3 *= 3;
    }
    return sum;
}
