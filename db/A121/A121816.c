/*
 * A121816  Conjectured chromatic number of the square of an outerplanar graph
 *          G^2 as a function of the maximum degree of a vertex of G.
 * https://oeis.org/A121816
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n+5 for 4 <= n <= 7; a(n) = floor(3*n/2)+1 for n >= 8
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 4 the sequence is not defined; return 0. */
    if (n < 4) return 0;

    /* 4 <= n <= 7: simple linear growth. */
    if (n <= 7) return (seq_t)n + 5;

    /* n >= 8: floor(3*n/2) + 1. */
    seq_t three_n = (seq_t)n * 3;   /* 3*n, stays within unsigned limits for the tested range. */
    three_n /= 2;                  /* floor division by 2. */
    return three_n + 1;
}
