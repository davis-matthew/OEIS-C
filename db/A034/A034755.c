/*
 * A034755  Dirichlet convolution of 3^(n-1) with Catalan numbers.
 * https://oeis.org/A034755
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|n} 3^(d-1) * Catalan(n/d - 1)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 18, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   catalan, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow3.h"
#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t result = 0;
    idx_t d;
    for (d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            idx_t nd = n / d;
            seq_t f = pow3(d - 1);           /* 3^(d-1) */
            seq_t g = catalan(nd - 1);       /* Catalan(n/d - 1) */
            result += f * g;
            if (nd != d) {                   /* counterpart divisor */
                seq_t f2 = pow3(nd - 1);     /* 3^(nd-1) = 3^(n/d-1) */
                seq_t g2 = catalan(d - 1);   /* Catalan(d - 1) */
                result += f2 * g2;
            }
        }
    }
    return result;
}
