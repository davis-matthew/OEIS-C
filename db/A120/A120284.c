/*
 * A120284  Numerator of absolute value of Sum_{k=1..n}
 *          (-1)^(k+1)*(2*k+1)*(Sum_{i=1..k} 1/i).
 * https://oeis.org/A120284
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = numerator( |∑_{k=1..n} (-1)^(k+1)·(2k+1)·H_k| ), where
 *            H_k = ∑_{i=1..k} 1/i (the k‑th harmonic number)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 18, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation keeps all partial results as fractions and reduces
   them on each step.  No floating point is used; all arithmetic is exact
   for the given seq_t type. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"   /* static seq_t gcd(seq_t a, seq_t b) */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined for n<1, but harmless */

    /* Harmonic number H_k = H_{k-1} + 1/k, stored as reduced fraction
       H_num / H_den.  Initially H_0 = 0/1. */
    seq_t H_num = 0;
    seq_t H_den = 1;

    /* Running sum S = sum_{k=1..n} (-1)^(k+1)*(2k+1)*H_k,
       stored as reduced fraction sum_num / sum_den (positive numerator
       and a separate sign flag). */
    seq_t sum_num = 0;      /* absolute value of numerator */
    seq_t sum_den = 1;
    bool sum_sign = true;   /* true ⇒ positive, false ⇒ negative */

    for (idx_t k = 1; k <= n; ++k)
    {
        /* Update harmonic number H_k = H_{k-1} + 1/k */
        seq_t k_seq   = (seq_t)k;
        seq_t new_num = H_num * k_seq + H_den;   /* H_num*k + H_den   */
        seq_t new_den = H_den * k_seq;
        seq_t g       = gcd(new_num, new_den);
        if (g != 0) {
            H_num = new_num / g;
            H_den = new_den / g;
        } else {
            H_num = new_num;
            H_den = new_den;
        }

        /* Term sign: positive for odd k, negative for even k */
        bool term_sign = (k & 1ULL);           /* true when k odd */

        /* term_abs = |(-1)^(k+1)*(2k+1)*H_k| = H_num*(2k+1) */
        seq_t two_k_plus_one = (seq_t)(2 * k + 1);
        seq_t term_abs = H_num * two_k_plus_one;

        /* Add term_abs/1 to sum_num/sum_den with appropriate sign.  To
           avoid overflow we add on a common denominator. */

        /* lcm(sum_den, H_den) = sum_den / gcd * H_den  */
        seq_t g2 = gcd(sum_den, H_den);
        seq_t lcm = (sum_den / g2) * H_den;

        /* Scale numerators to the common denominator */
        seq_t mul_sum  = lcm / sum_den;
        seq_t mul_term = lcm / H_den;          /* term denominator is 1 */

        seq_t left  = sum_num  * mul_sum;
        seq_t right = term_abs * mul_term;     /* (+/-) term numerator */

        /* Combine with signs */
        seq_t new_abs;
        bool new_sign;
        if (sum_sign == term_sign) {
            new_abs   = left + right;
            new_sign  = sum_sign;
        } else {
            if (left >= right) {
                new_abs   = left - right;
                new_sign  = sum_sign;
            } else {
                new_abs   = right - left;
                new_sign  = term_sign;
            }
        }

        /* Reduce the resulting fraction */
        seq_t g3 = gcd(new_abs, lcm);
        if (g3 != 0) {
            new_abs /= g3;
            lcm      /= g3;
        }

        sum_num  = new_abs;
        sum_den  = lcm;
        sum_sign = new_sign;
        if (sum_num == 0)          /* zero is treated as positive */
            sum_sign = true;
    }

    /* The required value is the absolute numerator of the final sum. */
    return sum_num;
}
