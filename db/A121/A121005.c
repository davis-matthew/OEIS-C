/*
 * A121005  Denominators of partial alternating sums of Catalan numbers scaled
 *          by powers of 1/125.
 * https://oeis.org/A121005
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator of r(n) where r(n) = sum_{k=0..n} Catalan(k)
 *            / 5^(3k) (in lowest terms) a(n) is the denominator of r(n)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   catalan, power5, seq_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/catalan.h"
#include "../utils/seq_one.h"
#include "../utils/power5.h"

seq_t a(idx_t n)
{
    /* r(0) = 1/1, so the denominator is 1. */
    if (n == 0)
        return power5((idx_t)0);   /* 1 */

    /* current numerator (in lowest terms) and exponent of 5 in the
       current denominator (i.e., denominator = 5^t).  Start with r(0)=1/1. */
    seq_t num = seq_one();   /* numerator of r(k-1) */
    unsigned int t   = 0;    /* exponent of 5 in the current denominator */

    /* Add terms for k = 1 .. n */
    for (idx_t k = 1; k <= n; ++k) {
        unsigned int a   = 3U * (unsigned int)k;        /* exponent of 5 in term k's denominator */
        unsigned int l   = (t > a) ? t : a;             /* common exponent */
        idx_t shift_num  = (idx_t)(l - t);
        idx_t shift_term = (idx_t)(l - a);

        /* scale existing numerator and term numerator to common denominator 5^l */
        seq_t scaled_num  = num  * power5(shift_num);
        seq_t scaled_term = catalan(k) * power5(shift_term);

        /* new numerator before reducing */
        seq_t new_num = scaled_num + scaled_term;

        /* cancel factors of 5 from the new numerator */
        unsigned int g = 0;
        seq_t temp = new_num;
        while ((temp % 5U) == 0U) {
            temp /= 5U;
            ++g;
        }

        num = temp;               /* reduced numerator */
        t   = l - g;               /* exponent of 5 in the new denominator */
    }

    /* current denominator is 5^t */
    return power5((idx_t)t);
}
