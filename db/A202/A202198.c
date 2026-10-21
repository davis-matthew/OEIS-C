/*
 * A202198  Number of (n+2) X 6 binary arrays avoiding patterns 001 and 101 in
 *          rows and columns.
 * https://oeis.org/A202198
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+6)*(n+5)*(n+4)*(n+3)*(n+2)^2/10
 * Range:     u8 none, u16 n <= 2, u32 n <= 36, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset handling – not used for the OEIS offset 1 */
    seq_t N   = (seq_t)n;
    seq_t p1  = N + 6;
    seq_t p2  = N + 5;
    seq_t p3  = N + 4;
    seq_t p4  = N + 3;
    seq_t p5  = N + 2;

    seq_t result = p1;
    result *= p2;
    result *= p3;
    result *= p4;

    seq_t sq = p5 * p5;   /* (n+2)^2 */
    result *= sq;

    return result / 10;
}
