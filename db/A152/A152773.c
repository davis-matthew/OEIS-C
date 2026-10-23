/*
 * A152773  3 times heptagonal numbers: a(n) = 3*n*(5*n-3)/2.
 * https://oeis.org/A152773
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n*(5*n-3)/2 (equivalent to (15*n^2 - 9*n)/2)
 * Range:     u8 n <= 4, u16 n <= 92, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    /* 15*n^2 - 9*n */
    seq_t val = (seq_t)15 * nn * nn;
    val -= (seq_t)9 * nn;
    return val / 2;
}
