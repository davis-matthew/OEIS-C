/*
 * A061485  Numerator of 1/(2n-1) + 3/(2n-3) + 5/(2n-5) + ...(2n-1)/1.
 * https://oeis.org/A061485
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) is the numerator of 1/(2n-1) + 3/(2n-3) + 5/(2n-5) + … +
 *            (2n-1)/1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset is 1, but guard */

    seq_t cur_num = (seq_t)0;      /* current numerator of H(n) = Σ 1/(2k-1) */
    seq_t cur_den = (seq_t)1;      /* current denominator of H(n) */

    for (idx_t k = 1; k <= n; ++k) {
        seq_t d = (seq_t)2 * k - (seq_t)1;           /* odd denominator 2k-1 */

        seq_t g1 = gcd(cur_den, d);                /* gcd of current denominator and d */

        /* Compute new denominator and numerator after adding 1/d */
        seq_t new_den = (cur_den / g1) * d;           /* lcm(cur_den, d) */
        seq_t new_num = cur_num * (d / g1) + (cur_den / g1);

        /* Reduce fraction */
        seq_t g2 = gcd(new_num, new_den);
        cur_num = new_num / g2;
        cur_den = new_den / g2;
    }

    /* Now H(n) = cur_num / cur_den.
       S(n) = 2n * H(n) - n  =>  numerator = n * (2*cur_num - cur_den) */
    seq_t n_seq   = (seq_t)n;
    seq_t num     = n_seq * ((seq_t)2 * cur_num - cur_den);
    seq_t den     = cur_den;
    seq_t g_final = gcd(num, den);

    return num / g_final;
}
