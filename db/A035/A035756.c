/*
 * A035756  Coordination sequence for C_19 lattice.
 * https://oeis.org/A035756
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = coefficient of x^(2n) in ((1+x)/(1-x))^19 a(n) =
 *            Σ_{k=0}^{min(19,2n)} C(19,k) * C(18+2n-k,18)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 5, u64 n <= 21, u128 n <= 251,
 *            big any n
 * Helpers:   binom_large18
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/binom_large18.h"

static const unsigned small_binom19[20] = {
    1, 19, 171, 969, 3876, 11628, 27132, 50388, 75582, 92378,
    92378, 75582, 50388, 27132, 11628, 3876, 969, 171, 19, 1
};

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t twon = n << 1;          /* 2*n */
    idx_t maxk = (twon < 19) ? twon : 19;
    for (idx_t k = 0; k <= maxk; ++k) {
        idx_t exp = twon - k;                     /* 2n - k */
        seq_t N = (seq_t)(18 + exp);              /* 18 + exp */
        seq_t binom18 = binom_large18(N);         /* C(18+exp,18) */
        seq_t term = (seq_t)small_binom19[k] * binom18;
        sum += term;
    }
    return sum;
}
