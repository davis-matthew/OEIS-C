/*
 * A168150  Inverse binomial transform of A026741.
 * https://oeis.org/A168150
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (-2)^(n-3) * n for n > 3 Initial values: a(0)=0, a(1)=1,
 *            a(2)=-1, a(3)=3 (equivalently a(n) = -4*a(n-1) - 4*a(n-2) for n
 *            ≥ 4)
 * Range:     i8 n <= 7, i16 n <= 14, i32 n <= 29, i64 n <= 60, i128 n <= 123,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n == 2) return -1;
    if (n == 3) return 3;

    idx_t k = n - 3;                     /* exponent for (-2) */
    seq_t sign = ((k & 1ULL) == 0) ? 1 : -1;  /* (-1)^k */
    seq_t pow2 = pow_int(2, k);          /* 2^k */
    seq_t prod = pow2 * (seq_t)n;        /* 2^k * n */
    return sign * prod;
}
