/*
 * A069052  Denominator of Sum_{i = 1..n} 1/i^5.
 * https://oeis.org/A069052
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, frac, nonn
 * Formula:   a(n) = denominator( Sum_{k=1..n} 1/k^5 )
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 10, u128 n <= 13,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Return 0 for n == 0 (out of range of the sequence). */
    if (n == 0) return 0;

    seq_t num = 0;          /* current numerator   */
    seq_t den = 1;          /* current denominator */

    for (idx_t k = 1; k <= n; ++k) {
        /* Compute k^5 as a seq_t. */
        seq_t k_seq = (seq_t)k;
        seq_t k5 = k_seq * k_seq;     /* k^2 */
        k5 *= k_seq;                  /* k^3 */
        k5 *= k_seq;                  /* k^4 */
        k5 *= k_seq;                  /* k^5 */

        /* lcm of den and k^5: den * (k5 / gcd(den,k5)) */
        seq_t g = gcd(den, k5);
        seq_t factor = k5 / g;        /* k5/g */
        seq_t add_term = den / g;     /* den/g */

        /* Add the new term: num/den + 1/k5 */
        seq_t new_num = num * factor + add_term;
        seq_t new_den = den * factor;

        /* Reduce the fraction */
        seq_t g2 = gcd(new_num, new_den);
        num = new_num / g2;
        den = new_den / g2;
    }
    return den;
}
