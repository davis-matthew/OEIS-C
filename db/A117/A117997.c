/*
 * A117997  Sum_{d|n} a(d) = n for n = 3^m (m >= 0) and for other n the sum is
 *          zero; i.e., the Möbius transform of [1, 0, 3, 0, 0, 0, 0, 0, 9,
 *          0,...].
 * https://oeis.org/A117997
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = μ(m) if 3∤n; otherwise a(n) = μ(m) * 2 * 3^(e-1) where e
 *            = ord_3(n) (>0) and m = n / 3^e (so 3∤m)
 * Range:     i8 n <= 242, i16 n <= 9799, i32 n <= 9799, i64 n <= 9799,
 *            i128 n <= 9799, big any n
 * Helpers:   mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mobius.h"

seq_t a(idx_t n)
{
    /* Compute the exponent of 3 in n and the reduced part m. */
    idx_t e = 0;
    idx_t x = n;
    while ((x % 3ULL) == 0ULL) {
        x /= 3ULL;
        ++e;
    }
    idx_t m = x;            /* 3∤m */

    seq_t mu = mobius(m);   /* μ(m) */

    if (e == 0) {            /* n not divisible by 3 */
        return mu;
    }

    /* Compute 3^(e-1).  e-1 is small (≤ 8 for n up to 10000). */
    seq_t p = 1;
    for (idx_t i = 0; i < e - 1; ++i) {
        p *= 3;
    }

    return mu * (seq_t)2 * p;
}
