/*
 * A035859  Coordination sequence for A_33 lattice.
 * https://oeis.org/A035859
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{d=1}^{33} C(34,d)*C(n-1,d-1)*C(n+33-d,n) (m=2n)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 11, u128 n <= 12,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)1;
    seq_t sum = 0;
    for (idx_t d = 1; d <= 33; ++d) {
        if (d > n)   /* binomial(n-1,d-1) would be zero */
            continue;
        seq_t c1 = binomial(34, d);          /* 34 choose d */
        seq_t c2 = binomial(n - 1, d - 1);   /* (n-1) choose (d-1) */
        seq_t c3 = binomial(n + 33 - d, n);  /* (n+33-d) choose n */
        sum += c1 * c2 * c3;
    }
    return sum;
}
