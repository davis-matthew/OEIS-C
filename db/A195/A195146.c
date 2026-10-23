/*
 * A195146  Concentric 16-gonal numbers.
 * https://oeis.org/A195146
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n^2 + 3*(-1)^n - 3)/2
 * Range:     u8 n <= 7, u16 n <= 127, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t sq = nn * nn;          /* n^2 */
    seq_t res;

    if ((n & 1U) == 0)           /* n even */
        res = (seq_t)4 * sq;     /* 4*n^2 */
    else
        res = (seq_t)4 * sq - (seq_t)3; /* 4*n^2 - 3 */

    return res;
}
