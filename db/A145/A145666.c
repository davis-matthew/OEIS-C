/*
 * A145666  a(n) = numerator of polynomial of genus 1 and level n for m = 7 :
 *          A[1,n](7).
 * https://oeis.org/A145666
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = Sum_{d=1}^{n-1} 7^(n-d)/d, a(1)=0
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n <= 1) return 0;

    seq_t num = 0;   /* numerator of S(1) */
    seq_t den = 1;   /* denominator of S(1) */

    for (idx_t i = 2; i <= n; ++i)
    {
        /* recurrence: S(i) = 7 * S(i-1) + 7/(i-1) */
        idx_t d = i - 1;                /* denominator of the second term */
        seq_t b = den;                  /* denominator of the first term */

        /* reduce when adding the two fractions */
        seq_t g = gcd(b, (seq_t)d);
        seq_t b_div = b / g;
        seq_t d_div = (seq_t)d / g;

        seq_t new_num = (seq_t)7 * num * d_div + (seq_t)7 * b_div;
        seq_t new_den = b * d_div;

        seq_t g2 = gcd(new_num, new_den);
        num = new_num / g2;
        den = new_den / g2;
    }

    return num;   /* the numerator of the reduced fraction */
}
