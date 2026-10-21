/*
 * A204243  Determinant of the n-th principal submatrix of A204242.
 * https://oeis.org/A204243
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1 - Sum_{k=2..n} 1/(2^k-1)) * Product_{k=2..n} (2^k-1)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    if (n == 1) return 1;

    seq_t P = 1;          /* P = Π_{k=2}^{current} (2^k-1)   */
    seq_t S = 0;          /* S = Σ_{k=2}^{current} Π_{j≠k} (2^j-1) */

    for (idx_t k = 2; k <= n; ++k) {
        seq_t oldP = P;
        seq_t d = pow2(k) - 1;   /* 2^k - 1 */

        P = oldP * d;            /* P_k = P_{k-1} * d            */
        S = S * d + oldP;        /* S_k = S_{k-1} * d + P_{k-1}   */
    }

    return P - S;                 /* a(n) = P_n - S_n              */
}
