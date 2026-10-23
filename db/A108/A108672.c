/*
 * A108672  a(n) = Sum{k=1 to n} sigma_{n-k+1}(k), where sigma_m(k) = sum{j|k}
 *          j^m.
 * https://oeis.org/A108672
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{k=1}^{n} σ_{n-k+1}(k) where σ_m(k) = ∑_{d|k} d^m
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 16, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq.h"

seq_t a(idx_t n) {
    seq_t sum = 0;
    for (idx_t d = 1; d <= n; ++d) {
        seq_t t = pow_seq((seq_t)d, d);          /* t = d^d */
        idx_t limit = n / d;                     /* number of multiples of d up to n */
        idx_t exp = n - d + 1;                   /* exponent for the first multiple (k=d) */
        seq_t p = pow_seq((seq_t)d, exp);        /* p = d^{exp} */

        for (idx_t m = 1; m <= limit; ++m) {
            sum += p;                            /* add contribution for k = d*m */
            if (m < limit) p /= t;               /* prepare next power: divide by d^d */
        }
    }
    return sum;
}
