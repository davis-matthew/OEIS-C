/*
 * A100692  Number of self-avoiding paths with n steps on a hexagonal lattice
 *          in the strip Z X {-1,0,1}.
 * https://oeis.org/A100692
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=3, a(2)=4; for n>=3: let k=(n-3)/4, r=(n-3)%4. Then
 *            a(4k+3)=4*2^k, a(4k+4)=6*2^k, a(4k+5)=a(4k+6)=10*2^k
 * Range:     u8 n <= 24, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)4;

    idx_t k = (n - 3) / 4;
    idx_t r = (n - 3) % 4;

    seq_t p = pow2(k);          /* 2^k */
    if (r == 0) return p * (seq_t)4;
    if (r == 1) return p * (seq_t)6;
    /* r is 2 or 3 */
    return p * (seq_t)10;
}
