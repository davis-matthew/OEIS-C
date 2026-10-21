/*
 * A217528  a(n) = (n-2)*(n-3)*2^(n-2)+2^n-2.
 * https://oeis.org/A217528
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-2)*(n-3)*2^(n-2) + 2^n - 2
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 25, u64 n <= 54, u128 n <= 116,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Handle small n explicitly to avoid non‑integer exponents. */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)6;

    /* (n-2)*(n-3)*2^(n-2) + 2^n - 2 */
    seq_t part1 = (seq_t)(n - 2) * (seq_t)(n - 3) * pow2(n - 2);
    seq_t part2 = pow2(n) - (seq_t)2;
    return part1 + part2;
}
