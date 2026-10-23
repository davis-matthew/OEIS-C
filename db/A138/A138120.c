/*
 * A138120  Concatenation of n digits 1, 2n-1 digits 0 and n digits 1.
 * https://oeis.org/A138120
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = (1/9)*(10^n - 1)*(1 + 10^(3*n-1))
 * Range:     u8 none, u16 n <= 1, u32 n <= 2, u64 n <= 5, u128 n <= 9,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                    /* offset is 1, but guard against 0 */

    seq_t pow10_n = pow10(n);        /* 10^n */
    seq_t part1 = pow10_n;           /* 10^n - 1 */
    part1 -= 1;

    idx_t exp3 = 3 * n;
    exp3 -= 1;                       /* 3*n - 1 */
    seq_t pow10_3n_minus1 = pow10(exp3);
    seq_t part2 = pow10_3n_minus1;   /* 1 + 10^(3*n-1) */
    part2 += 1;

    seq_t prod = part1 * part2;      /* (10^n - 1)*(1 + 10^(3*n-1)) */
    seq_t result = prod / 9;         /* divide by 9, exact division */
    return result;
}
