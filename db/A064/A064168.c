/*
 * A064168  Sum of numerator and denominator in n-th harmonic number, 1 + 1/2
 *          + 1/3 +...+ 1/n.
 * https://oeis.org/A064168
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = numerator(H(n)) + denominator(H(n))
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   where H(n) = 1 + 1/2 + … + 1/n  */
#include <stddef.h>

/* Euclidean algorithm for any seq_t */
static seq_t _gcd_seq_t(seq_t a, seq_t b)
{
    while (b != 0)
    {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* out of range, behaviour irrelevant */

    seq_t num = 1; /* numerator of H(1) */
    seq_t den = 1; /* denominator of H(1) */

    for (idx_t k = 2; k <= n; ++k)
    {
        seq_t k_seq   = (seq_t)k; /* cast index to seq_t */
        seq_t den_old = den;      /* keep old denominator before update */

        den = den * k_seq;          /* new denominator */
        num = num * k_seq + den_old;/* new numerator before reduction */

        seq_t g = _gcd_seq_t(num, den);
        if (g != 1)
        {
            num /= g;
            den /= g;
        }
    }

    return num + den;
}
