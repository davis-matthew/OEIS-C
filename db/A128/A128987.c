/*
 * A128987  a(n) = (n^3 - n^2)*4^n.
 * https://oeis.org/A128987
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^3 - n^2)*4^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 25, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute a(n) = (n^3 - n^2)*4^n for n >= 1.
   seq_t and idx_t are provided by the build system.
*/
seq_t a(idx_t n)
{
    /* compute 4^n in seq_t */
    seq_t pow4 = 1;
    for (idx_t i = 0; i < n; ++i)
        pow4 *= 4;            /* multiplication in seq_t */

    /* compute n^2 and then n^3 - n^2 = n^2*(n-1) */
    seq_t sn   = (seq_t)n;
    seq_t n_sq = sn * sn;      /* n^2 */
    seq_t n_mul_minus_one = sn - 1; /* n-1 (for n>=1) */
    seq_t diff = n_sq * n_mul_minus_one;   /* n^3 - n^2 */

    return diff * pow4;
}
