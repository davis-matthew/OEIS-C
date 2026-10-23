/*
 * A027929  a(n) = T(n, 2*n-6), T given by A027926.
 * https://oeis.org/A027929
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..3} binomial(n-k, 6-2*k) (offset 3)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 118, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n < 3) return 0;  /* offset not defined for n < 3; return 0 */

    seq_t sum = 0;
    for (idx_t k = 0; k <= 3; ++k) {
        idx_t coeff = 6 - 2 * k;
        if (n >= k && (n - k) >= coeff) {
            sum += binomial(n - k, coeff);
        }
    }
    return sum;
}
