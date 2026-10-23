/*
 * A179088  Positive integers of the form (2*m^2+1)/11.
 * https://oeis.org/A179088
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (22*n*(n-1) - 5*(2*n-1)*(-1)^n + 7)/4
 * Range:     u8 n <= 4, u16 n <= 108, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t part1 = (seq_t)22 * nn * (nn - 1);
    seq_t tmp   = (seq_t)2 * nn - 1;
    seq_t part2 = (seq_t)5 * tmp;
    if ((n & 1) == 0) {          /* even n */
        part1 -= part2;
    } else {                     /* odd n */
        part1 += part2;
    }
    part1 += 7;
    return part1 / 4;
}
