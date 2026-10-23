/*
 * A028947  Difference between P(n) and nearest integer to P(n-1)^2 / P(n-2),
 *          where P(i) =i-th prime.
 * https://oeis.org/A028947
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = P(n) - nearestint(P(n-1)^2 / P(n-2)) (n ≥ 3)
 * Range:     i8 n <= 32, i16 n <= 80, i32 n <= 80, i64 n <= 80, i128 n <= 80,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* nearestint(x) is computed as floor(x + ½).                     */
#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n < 3)                     /* indices below the offset are undefined */
        return 0;

    seq_t pn  = (seq_t)get_prime(n);
    seq_t pn1 = (seq_t)get_prime(n - 1);
    seq_t pn2 = (seq_t)get_prime(n - 2);

    /* round(Pn1^2 / Pn2) as floor((Pn1^2 + Pn2/2)/Pn2)   */
    seq_t rounded = (pn1 * pn1 + pn2 / 2) / pn2;

    return pn - rounded;
}
