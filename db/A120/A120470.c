/*
 * A120470  a(n) = 2*4^n + (-1)^n*2^(n-1).
 * https://oeis.org/A120470
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*4^n + (-1)^n*2^(n-1)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1.  Return 0 for n == 0
     * to avoid undefined behavior when called with the offset. */
    if (n == 0)
        return 0;

    /* Compute the two parts of the formula using the provided
     * big‑integer power routine. */
    seq_t part1 = pow_uint(4, n);
    part1 = part1 * 2;                 /* 2 * 4^n  */

    seq_t part2 = pow_uint(2, n - 1);  /* 2^(n-1)  */

    /* Add or subtract the second part depending on the parity of n. */
    if (n & 1)          /* odd n → subtract 2^(n-1) */
        return part1 - part2;
    else                /* even n → add 2^(n-1) */
        return part1 + part2;
}
