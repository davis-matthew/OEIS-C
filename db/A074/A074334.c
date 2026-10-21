/*
 * A074334  a(n) = Sum_{r=1..n} r^4*binomial(n,r)^2.
 * https://oeis.org/A074334
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^2 * (n^3 + n^2 - 3*n - 1) * C(n-2) (n > 1) a(0) = 0,
 *            a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 26, u128 n <= 56,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;

    /* n >= 2 */
    seq_t sn   = (seq_t)n;
    seq_t n_sq = sn * sn;                   /* n^2 */
    seq_t n_cu = n_sq * sn;                 /* n^3 */
    seq_t temp = n_cu + n_sq - 3 * sn - 1;  /* n^3 + n^2 - 3*n - 1 */

    seq_t cat = catalan(n - 2);            /* Catalan number C(n-2) */
    seq_t prod = n_sq * temp;              /* (n^2)*(...) */
    return prod * cat;                     /* final result */
}
