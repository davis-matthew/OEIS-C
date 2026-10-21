/*
 * A228701  a(n) = 2^(n-1)*(3^n-2^(n+1)+1).
 * https://oeis.org/A228701
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-1)*(3^n-2^(n+1)+1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 26,
 *            big any n
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t p2_n_1   = pow2(n - 1);
    seq_t p3_n     = pow3(n);
    seq_t p2_n_p1  = pow2(n + 1);
    seq_t term = p3_n - p2_n_p1 + 1;
    return p2_n_1 * term;
}
