/*
 * A022929  Number of 3^m between 4^n and 4^(n+1).
 * https://oeis.org/A022929
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ⌊α (n+1)⌋ – ⌊α n⌋ where α = 2 / log₂(3)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) counts 3ᵐ such that 4ⁿ < 3ᵐ < 4ⁿ⁺¹.  It is always 1 or 2. */
#include <math.h>

seq_t a(idx_t n)
{
    /* α = 2 / log₂(3) – irrational, used to step through the
       Beatty sequence floor(α n).  Long double gives ~19 decimal
       digits, enough for n ≤ 10⁴ with a tiny epsilon margin. */
    static const long double alpha = 2.0L / log2l(3.0L);
    static const long double eps = 1e-12L;   /* safety margin */

    idx_t f_n   = (idx_t)floorl(alpha * (long double)n      + eps);
    idx_t f_n1  = (idx_t)floorl(alpha * (long double)(n+1) + eps);

    return (seq_t)(f_n1 - f_n);
}
