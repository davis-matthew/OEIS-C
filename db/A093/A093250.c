/*
 * A093250  a(n) is the largest number such that all of a(n)'s length-n
 *          substrings are distinct and divisible by 50.
 * https://oeis.org/A093250
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(1) = 0 for n ≥ 2 : a(n) = 10^(2n-1) – 5·10^n (The number is
 *            (n-2) copies of '9', then '5', followed by n zeros.)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;                     // a(1) = 0 (a(0) never queried)

    /* 10^(2n-1) */
    seq_t p1 = pow10(2 * n - 1);
    /* 10^n */
    seq_t p2 = pow10(n);

    return p1 - 5 * p2;
}
