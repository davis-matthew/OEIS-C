/*
 * A064199  a(n) = 9*(n-2)^2*(n^2-2*n-1)/2.
 * https://oeis.org/A064199
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*(n-2)^2*(n^2-2*n-1)/2
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 176, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n_minus_2 = nn - 2;
    seq_t part1 = n_minus_2 * n_minus_2;           // (n-2)^2
    seq_t part2 = nn * nn - 2 * nn - 1;            // n^2 - 2*n - 1
    seq_t temp  = part1 * part2;                  // (n-2)^2 * (n^2-2*n-1)
    seq_t res   = temp * 9;                        // multiply by 9
    res = res / 2;                                 // divide by 2
    return res;
}
