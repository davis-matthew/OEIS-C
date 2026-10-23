/*
 * A152764  Bisection of A138144.
 * https://oeis.org/A152764
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(1)=1, a(2)=111, a(n)=11+11*10^(2*n-3) for n>2 (this also
 *            satisfies a(n)=101*a(n-1)-100*a(n-2) for n>4)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 12,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined for offset 1, harmless */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)111;

    idx_t e = 2 * n - 3;                 /* exponent for 10 */
    seq_t p = pow10(e);                  /* p = 10^(2n-3) */
    seq_t result = (seq_t)11 * p + (seq_t)11;  /* 11 + 11*10^(2n-3) */
    return result;
}
