/*
 * A008514  4-dimensional centered cube numbers.
 * https://oeis.org/A008514
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^4 + (n+1)^4
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 214, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn  = (seq_t)n;
    seq_t nn1 = nn + 1;

    seq_t nn_sq  = nn * nn;          /* n^2            */
    seq_t nn4    = nn_sq * nn_sq;     /* n^4            */
    seq_t nn1_sq = nn1 * nn1;        /* (n+1)^2        */
    seq_t nn1_4  = nn1_sq * nn1_sq;   /* (n+1)^4        */

    return nn4 + nn1_4;
}
