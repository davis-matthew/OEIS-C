/*
 * A103349  Numerator of Sum_{k=1..n} 1/k^8 = zeta(8,n).
 * https://oeis.org/A103349
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = numerator( ∑_{k=1..n} 1/k^8 )
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 12,
 *            big any n
 * Helpers:   gcd_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  (offset 1)                                                    */

#include <stdint.h>        /* for uint64_t, if needed */
#include "../utils/gcd_seq.h"

seq_t a(idx_t n)
{
    seq_t num = 0;   /* numerator of the running sum */
    seq_t den = 1;   /* denominator of the running sum   */

    for (idx_t k = 1; k <= n; ++k) {
        /* compute d = k^8  */
        seq_t base = (seq_t)k;
        seq_t d = 1;
        for (int i = 0; i < 8; ++i) d *= base;

        /* add 1/d to the current fraction num/den  */
        seq_t g = gcd_seq(den, d);            /* gcd(den, d) */
        seq_t mul = d / g;                    /* d/g          */
        seq_t new_num = num * mul + den / g;  /* numerator    */
        seq_t new_den = den * mul;            /* denominator  */

        /* reduce the fraction again  */
        seq_t g2 = gcd_seq(new_num, new_den);
        num = new_num / g2;
        den = new_den / g2;
    }
    return num;   /* numerator after all reductions */
}
