/*
 * A015294  Gaussian binomial coefficient [ n,4 ] for q = -8.
 * https://oeis.org/A015294
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Gaussian binomial coefficient [n,4] with q = -8 * =
 *            ∏_{i=0}^{3} (1 - (-8)^(n-i)) / (1 - (-8)^(4-i)) * This
 *            implementation reduces the fraction step‑by‑step to avoid *
 *            overflow on 64‑bit builds; it works with arbitrary precision *
 *            builds as well
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   big_pow, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* OEIS offset 4: a(4) is the first listed term */
    if (n < 4)
        return 0;

    seq_t num = 1;   /* current numerator of the running product */
    seq_t den = 1;   /* current denominator of the running product */
    int  sign = 1;   /* keeps track of the overall sign; should be +1 */

    for (int i = 0; i < 4; ++i) {
        idx_t exp_num = n - i;      /* exponent in the numerator factor */
        idx_t exp_den = 4 - i;      /* exponent in the denominator factor */

        /* 8^exp_num and 8^exp_den as seq_t */
        seq_t pow_num = big_pow((seq_t)8, (seq_t)exp_num);
        seq_t pow_den = big_pow((seq_t)8, (seq_t)exp_den);

        /* absolute values of the factors 1 - (-8)^exp */
        seq_t abs_num = (exp_num & 1u) ? (pow_num + 1) : (pow_num - 1);
        seq_t abs_den = (exp_den & 1u) ? (pow_den + 1) : (pow_den - 1);

        /* signs: 1 - (-8)^exp is negative for even exp, positive for odd */
        int  sign_num = (exp_num & 1u) ? 1 : -1;
        int  sign_den = (exp_den & 1u) ? 1 : -1;
        int  term_sign = sign_num * sign_den;

        if (term_sign < 0)
            sign = -sign;          /* toggle sign */

        /* Reduce abs_num with the current denominator first */
        seq_t g1 = gcd(abs_num, den);
        abs_num /= g1;
        den     /= g1;

        /* Multiply numerator by the reduced abs_num */
        num *= abs_num;

        /* Reduce the new numerator with the current denominator element abs_den */
        seq_t g2 = gcd(num, abs_den);
        num      /= g2;
        abs_den   /= g2;

        /* Update the denominator with the remaining part of abs_den */
        den *= abs_den;
    }

    /* At this point den should divide num exactly; final division */
    seq_t gfinal = gcd(num, den);    /* should be equal to den for integer result */
    num /= gfinal;
    den /= gfinal;                   /* den should become 1 */

    /* The overall sign for n >= 4 is always positive, but keep the check */
    (void)sign;
    return num;
}
