/*
 * A035773  Coordination sequence for C_36 lattice.
 * https://oeis.org/A035773
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 10, u128 n <= 42,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A035773: a(n) = coefficient of x^(2n) in ((1+x)/(1-x))^36
   Equivalent formula: a(n) = Σ_{i=0}^{min(36,2n)} C(36,i) * C(35+2n-i,35)   */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    const idx_t EXP   = 36;               /* exponent of generating function */
    const idx_t SHIFT = 35;               /* second binomial shift */
    idx_t      two_n = n + n;             /* 2 * n (safe for n up to 10000) */

    /* upper bound for i in the sum: i ≤ EXP and i ≤ 2n */
    idx_t i_max = (two_n < EXP) ? two_n : EXP;

    seq_t sum = 0;
    for (idx_t i = 0; i <= i_max; ++i) {
        seq_t term = binom(EXP, i) * binom(SHIFT + two_n - i, SHIFT);
        sum += term;
    }
    return sum;
}
