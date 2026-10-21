/*
 * A060550  a(n) is the number of distinct patterns (modulo geometric
 *          D_3-operations) with no other than strict 120-degree rotational
 *          symmetry which can be formed by an equilateral triangular
 *          arrangement of closely packed black and white cells satisfying the
 *          local matching rule of Pascal's triangle modulo 2,...
 * https://oeis.org/A060550
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^(⌊n/3⌋ + (n mod 3 mod 2) – 1) – 2^(⌊(n+3)/6⌋ + d(n) –
 *            1), where d(n) = 1 if n mod 6 = 1, else 0
 * Range:     u8 n <= 27, u16 n <= 51, u32 n <= 99, u64 n <= 195,
 *            u128 n <= 387, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* exponent for the first power */
    int e1 = (int)(n / 3) + ((int)(n % 3) & 1) - 1;

    /* exponent for the second power */
    int e2 = (int)((n + 3) / 6) + ((int)(n % 6) == 1 ? 1 : 0) - 1;

    /* 2^e with negative e treated as 0 */
    seq_t v1 = 0, v2 = 0;
    if (e1 >= 0)
        v1 = pow2((idx_t)e1);
    if (e2 >= 0)
        v2 = pow2((idx_t)e2);

    /* subtraction is modulo the width of seq_t, so it remains correct
       even when one term overflows its type. */
    return v1 - v2;
}
