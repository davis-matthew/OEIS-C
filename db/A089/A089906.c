/*
 * A089906  Sum of digits of numbers between 0 and (4/9)*(10^n-1).
 * https://oeis.org/A089906
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (2*(-55*(-1+10^n)+9*(10+9*10^n)*n))/81
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 6, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* Compute 10^n.  pow10 returns a value of type seq_t. */
    seq_t T = pow10(n);

    /* part1 = 55 * (T - 1)  == 55*T - 55 */
    seq_t part1 = 55 * (T - 1u);

    /* part2 = 9 * ((10 + 9*T) * n)  == 9*(10+9*T)*n */
    seq_t inside = 10 + (9 * T);
    seq_t part2 = 9 * inside * n;

    /* sum = part2 - part1  == 90n + 81nT - 55T + 55  */
    seq_t sum = part2 - part1;

    /* Finally a(n) = (sum / 81) * 2.
       The division is exact because part2 - part1 = a(n)*81/2
       (see the derivation in the comment header). */
    seq_t res = (sum / 81u) * 2u;

    return res;
}
