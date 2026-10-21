/*
 * A052701  a(0) = 0; for n >= 1, a(n) = 2^(n-1)*C(n-1), where C(n) =
 *          A000108(n) Catalan numbers, n>0.
 * https://oeis.org/A052701
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^(n-1) * C(n-1) for n >= 1, a(0) = 0
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 24, u128 n <= 46,
 *            big any n
 * Helpers:   catalan, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t p = pow2(n - 1);     /* 2^(n-1) */
    seq_t c = catalan(n - 1);  /* Catalan(n-1) */
    return p * c;
}
