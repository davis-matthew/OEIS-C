/*
 * A178826  Partial sums of floor(7^n/9).
 * https://oeis.org/A178826
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = floor((7*7^n - 24*n - 7) / 54)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence used:  a(1)=0, a(2)=5, a(3)=43,
 * a(n)=a(n-3)+ (19*7^(n-2)-4)/3   for n>3.
 */

#include "../utils/big_pow.h"   /* Provides big_pow(seq_t, seq_t) if needed */

seq_t a(idx_t n)
{
    if (n == 1) return 0;
    if (n == 2) return 5;
    if (n == 3) return 43;

    /* Track last three terms, sliding window: a1=a(n-3), a2=a(n-2), a3=a(n-1) */
    seq_t a1 = 0, a2 = 5, a3 = 43;
    /* 7^(n-2) for current n=4 is 7^2 = 49 */
    seq_t pow = (seq_t)49;

    for (idx_t k = 4; k <= n; ++k) {
        /* term = (19*pow - 4)/3  */
        seq_t term = ((seq_t)19 * pow - 4) / 3;
        seq_t ak = a1 + term;      /* a(k) = a(k-3) + term */

        /* shift the window */
        a1 = a2;
        a2 = a3;
        a3 = ak;

        /* advance pow to 7^(k-1) for next iteration (k+1 => exponent k-1+1 = k) */
        pow *= 7;
    }
    return a3;
}
