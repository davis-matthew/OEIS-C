/*
 * A037449  Discriminant of quadratic field Q(sqrt(n)).
 * https://oeis.org/A037449
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Discriminant of Q(sqrt(n)) * a(n) = squarefree(n) if
 *            squarefree(n) ≡ 1 (mod 4) * 4 * squarefree(n) otherwise * where
 *            squarefree(n) is the product of prime factors of n with odd
 *            exponent
 * Range:     u8 n <= 65, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* compute the squarefree part of n */
    idx_t tmp = n;
    seq_t sf = 1;
    int e = 0;

    /* handle factor 2 */
    if (tmp % 2 == 0) {
        e = 0;
        while (tmp % 2 == 0) {
            tmp /= 2;
            ++e;
        }
        if (e & 1) /* odd exponent */
            sf *= (seq_t)2;
    }

    /* handle odd factors */
    for (idx_t i = 3; i * i <= tmp; i += 2) {
        if (tmp % i == 0) {
            e = 0;
            while (tmp % i == 0) {
                tmp /= i;
                ++e;
            }
            if (e & 1)
                sf *= (seq_t)i;
        }
    }

    /* leftover prime > sqrt(original n) */
    if (tmp > 1)
        sf *= (seq_t)tmp; /* exponent = 1 (odd) */

    /* final discriminant */
    if (sf % (seq_t)4 == 1)
        return sf;
    else
        return (seq_t)4 * sf;
}
