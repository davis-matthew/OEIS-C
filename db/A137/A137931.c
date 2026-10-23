/*
 * A137931  Sum of the principal diagonals of a 2n X 2n square spiral.
 * https://oeis.org/A137931
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n - 1 + (2*n+1)*(8*n^2 - n + 3)/3
 * Range:     u8 n <= 3, u16 n <= 22, u32 n <= 930, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    seq_t nn   = (seq_t)n;
    seq_t part1 = (seq_t)2 * nn + (seq_t)1;               // 2*n + 1
    seq_t part2 = (seq_t)8 * nn * nn - nn + (seq_t)3;     // 8*n^2 - n + 3

    /* compute (part1 * part2) / 3 without overflow */
    seq_t g1   = gcd(part1, (seq_t)3);
    seq_t part1_red = part1 / g1;
    seq_t d1   = (seq_t)3 / g1;

    seq_t g2   = gcd(part2, d1);
    seq_t part2_red = part2 / g2;
    seq_t d2   = d1 / g2;   /* should be 1 */

    seq_t prod   = part1_red * part2_red;  /* division handled above */
    seq_t result = nn - (seq_t)1 + prod;
    return result;
}
