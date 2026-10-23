/*
 * A127815  a(n) = denominator of b(n), where b(1) = 2, b(n) = b(n-1) -
 *          1/b(n-1).
 * https://oeis.org/A127815
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, frac, nonn
 * Formula:   a(n) = denominator of b(n), where * b(1) = 2, * b(n) = b(n-1) -
 *            1 / b(n-1) (n >= 2) * * The implementation iteratively evaluates
 *            the fraction * b(n) = p/q in lowest terms, keeping p and q as
 *            seq_t. * It returns q (the denominator) when the desired index
 *            is reached. * * The function works for any...
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_gcd.h"

seq_t a(idx_t n)
{
    /* b(1) = 2/1 */
    seq_t num = 2;     /* numerator */
    seq_t den = 1;     /* denominator (always positive) */

    if (n == 1)
        return den;  /* 1 */

    for (idx_t i = 2; i <= n; ++i) {
        /* b = num/den  ->  next = b - 1/b
         *      = (num^2 - den^2) / (num*den)   (reduced)
         */
        seq_t num_square = num * num;
        seq_t den_square = den * den;

        seq_t new_num_abs;
        if (num_square >= den_square)
            new_num_abs = num_square - den_square;
        else
            new_num_abs = den_square - num_square;   /* absolute difference */

        seq_t new_den = num * den;

        seq_t g = seq_gcd(new_num_abs, new_den);
        num = new_num_abs / g;
        den = new_den / g;
        /* den is always positive; no sign tracking needed for the result */
    }

    return den;
}
