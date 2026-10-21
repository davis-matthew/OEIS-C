/*
 * A061693  Generalized Bell numbers.
 * https://oeis.org/A061693
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ½ * Σ_{k=1}^{n-1} binom(n,k)^3 (OEIS A000172(n)/2-1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;            /* a(1) = 0 */

    seq_t sum   = 0;
    seq_t binom = 1;          /* binom(n,0) */

    for (idx_t k = 1; k < n; ++k) {
        /* binom = binom * (n - k + 1) / k */
        binom = (binom * (seq_t)(n - k + 1)) / (seq_t)k;

        /* cube = binom^3 */
        seq_t cube = binom * binom;
        cube *= binom;

        sum += cube;
    }
    return sum / 2;           /* ½ * Σ binom^3 */
}
