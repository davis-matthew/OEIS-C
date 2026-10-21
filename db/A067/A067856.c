/*
 * A067856  Sum_{n >= 1} a(n)/n^s = 1/(Sum_{n >= 1} (-1)^(n + 1)/n^s).
 * https://oeis.org/A067856
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = μ(n) for odd n; a(n) = μ(m) * 2^(k-1) when n = m * 2^k
 *            with m odd and k ≥ 1
 * Range:     i8 n <= 255, i16 n <= 9447, i32 n <= 9447, i64 n <= 9447,
 *            i128 n <= 9447, big any n
 * Helpers:   mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the helper function: seq_t mobius(idx_t n) */

/* Header providing mobius() */
#include "../utils/mobius.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1, safe return */

    /* Separate n into odd part m and exponent k of 2 */
    idx_t k = 0;
    idx_t tmp = n;
    while ((tmp & 1) == 0) {      /* while even */
        ++k;
        tmp >>= 1;                /* divide by 2 */
    }
    idx_t m = tmp;                /* odd part of n */

    seq_t mu = mobius(m);         /* μ(m), the Möbius function */

    if (k == 0) {                 /* n is odd */
        return mu;
    }

    /* Compute 2^(k-1) in type seq_t (avoid bitwise shift on arbitrary int) */
    seq_t pow2 = 1;
    for (idx_t i = 1; i < k; ++i) {
        pow2 *= 2;
    }

    return mu * pow2;
}
