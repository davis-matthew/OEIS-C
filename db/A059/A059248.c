/*
 * A059248  Numerator of 1/F(1) + 1/F(2) + 1/F(3) + ... + 1/F(n), where F(n)
 *          is the n-th Fibonacci number (A000045).
 * https://oeis.org/A059248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = numerator of \sum_{i=1}^{n} 1/F_i, where F_i is the i‑th
 *            Fibonacci number (F_1 = F_2 = 1)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 16, u128 n <= 23,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Edge case: sequence is defined for n >= 1, but return 0 for n == 0 */
    if (n == 0)
        return 0;

    seq_t num = 0;      /* current numerator of the sum */
    seq_t den = 1;      /* current denominator of the sum */

    /* Fibonacci numbers iteratively: F1 = 1, F2 = 1 */
    seq_t f_prev = 1;   /* F(k-1) */
    seq_t f_curr = 1;   /* F(k)   */

    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t fk;
        if (k == 1)
            fk = f_prev;
        else if (k == 2)
            fk = f_curr;
        else
        {
            fk = f_prev + f_curr;  /* F(k) = F(k-2) + F(k-1) */
            f_prev = f_curr;
            f_curr = fk;
        }

        /* Add 1/fk to the current sum: (num/den) + 1/fk = (num*fk + den)/(den*fk) */
        seq_t new_num = num * fk + den;
        seq_t new_den = den * fk;

        /* Reduce the fraction */
        seq_t g = gcd(new_num, new_den);
        num = new_num / g;
        den = new_den / g;
    }

    return num;
}
