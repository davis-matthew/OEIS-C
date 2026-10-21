/*
 * A074021  Number of elements of GF(7^n) with trace 1 and subtrace 4.
 * https://oeis.org/A074021
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 7^(n-2) + s(n) * 7^(floor((n-2)/2)) where the coefficient
 *            s(n) is defined by: - if n is even: let k = n/2. s(n) = +1 if k
 *            is odd, -1 if k is even. - if n is odd: let t = (n-1)/2. if t %
 *            3 == 0 -> s(n) = 0 else if t % 6 == 1 or t % 6 == 4 -> s(n) = -1
 *            else s(n) = +1 For n = 1 a(1) = 0. This...
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;

    idx_t exp1 = n - 2;              /* exponent for the main term 7^(n-2)   */
    idx_t exp2 = (n - 2) / 2;         /* exponent for the correction term     */
    seq_t term   = big_pow((seq_t)7, (seq_t)exp1);     /* 7^(n-2) */

    /* compute the sign s(n) according to the rules above */
    int sign = 1;
    if (n & 1) {                     /* n is odd */
        idx_t t = (n - 1) / 2;
        if (t % 3 == 0) {
            sign = 0;
        } else {
            idx_t m = t % 6;
            if (m == 1 || m == 4)
                sign = -1;
            else                    /* m == 2 or m == 5 */
                sign = +1;
        }
    } else {                         /* n is even */
        idx_t k = n / 2;
        sign = (k % 2 == 1) ? +1 : -1;
    }

    if (sign != 0) {
        seq_t corr = big_pow((seq_t)7, (seq_t)exp2);  /* 7^(floor((n-2)/2)) */
        if (sign > 0)
            term += corr;
        else
            term -= corr;            /* term is always ≥ corr for all n≥2 */
    }

    return term;
}
