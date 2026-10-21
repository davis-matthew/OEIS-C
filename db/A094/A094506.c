/*
 * A094506  Numerator of I(n) = (-1) * Integral_{x=0..4^n} (1-x^(3/2)) dx.
 * https://oeis.org/A094506
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^(5n+1) - 20*2^(2n-2) (for n >= 1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static seq_t pow2(idx_t e)
{
    /* Compute 2^e.  Avoid shifting beyond the width of seq_t. */
    if (e < 64) {
        return ((seq_t)1 << e);
    }
    seq_t r = 1;
    for (idx_t i = 0; i < e; ++i) {
        r *= 2;
    }
    return r;
}

seq_t a(idx_t n)
{
    /* Ensure n >= 1 (caller guarantees it, but no harm in handling 0). */
    if (n == 0) return 0;

    seq_t term1 = pow2((idx_t)(5 * n + 1));    /* 2^(5n+1) */
    seq_t term2 = pow2((idx_t)(2 * n - 2));    /* 2^(2n-2) */
    term2 *= (seq_t)20;                       /* 20 * 2^(2n-2) */
    return term1 - term2;                     /* a(n) */
}
