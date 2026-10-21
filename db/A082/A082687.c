/*
 * A082687  Numerator of Sum_{k=1..n} 1/(n+k).
 * https://oeis.org/A082687
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn, easy, changed
 * Formula:   a(n) = numerator(∑_{k=1..n} 1/(n+k))
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_gcd.h"

seq_t a(idx_t n)
{
    /* sequence is 1‑indexed; values for n < 1 are arbitrary */
    if (n == 0)
        return 0;

    seq_t num = 0;   /* current numerator of the sum */
    seq_t den = 1;   /* current denominator (reduced) */

    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t d = (seq_t)(n + i);   /* denominator of the new term 1/(n+i) */

        /* add 1/d to the running fraction num/den */
        seq_t new_num = num * d + den;   /* numerator of the added fraction */
        seq_t new_den = den * d;         /* denominator of the sum before reduction */

        /* reduce the fraction */
        seq_t g = seq_gcd(new_num, new_den);
        num = new_num / g;
        den = new_den / g;
    }

    return num;   /* numerator of the reduced fraction */
}
