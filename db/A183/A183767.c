/*
 * A183767  1/32 the number of (n+1) X 5 binary arrays with equal numbers of 2
 *          X 2 subblocks with sum mod two being 0 and 1.
 * https://oeis.org/A183767
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^n * binomial(4*n, 2*n)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binom.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* OEIS offset is 1; for n=0 we return 0 (unused) */
    if (n == 0)
        return (seq_t)0;

    /* compute binomial(4*n, 2*n) */
    idx_t total = 4 * n;
    idx_t half = 2 * n;
    seq_t bin = binom(total, half);

    /* compute 2^n */
    seq_t pow = pow2(n);

    /* result */
    return bin * pow;
}
