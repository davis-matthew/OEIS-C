/*
 * A136921  Numbers k such that k and k^2 use only the digits 0, 2, 6 and 7.
 * https://oeis.org/A136921
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = 0 for n = 1, otherwise a(n) = 26 * 10^(n-2)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This satisfies the requirement that both k and k^2 use only
    the digits 0, 2, 6 and 7. */
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)0;
    /* For n ≥ 2, compute 26 × 10^(n-2). */
    seq_t p = pow10(n - 2);
    return (seq_t)26 * p;
}
