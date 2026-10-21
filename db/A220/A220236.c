/*
 * A220236  Binary palindromic numbers with only two 0 bits, both in the
 *          middle.
 * https://oeis.org/A220236
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = 2^(2*n + 2) - 2^(n + 1) - 2^n - 1
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"   /* provides seq_t pow2(idx_t exp) */

seq_t a(idx_t n)
{
    if (n == 0)         /* index 0 is outside the OEIS offset, but guard against it */
        return (seq_t)0;

    idx_t e1 = 2 * n + 2; /* exponent for 2^(2n+2) */
    idx_t e2 = n + 1;     /* exponent for 2^(n+1) */
    idx_t e3 = n;         /* exponent for 2^n */

    seq_t term1 = pow2(e1);
    seq_t term2 = pow2(e2);
    seq_t term3 = pow2(e3);

    seq_t res = term1 - term2 - term3 - (seq_t)1;
    return res;
}
