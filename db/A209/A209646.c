/*
 * A209646  Number of n X 4 0..1 arrays avoiding 0 0 1 and 1 0 0 horizontally
 *          and 0 0 1 and 1 0 1 vertically.
 * https://oeis.org/A209646
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*n^3 + (9/2)*n^2 - (9/2)*n
 * Range:     u8 n <= 2, u16 n <= 19, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;          /* n */
    seq_t n2 = nn * nn;          /* n^2 */
    seq_t n3 = n2 * nn;          /* n^3 */
    seq_t t1 = (seq_t)9 * n3;                                    /* 9*n^3 */
    seq_t t2 = ((seq_t)9 * n2 - (seq_t)9 * nn) / (seq_t)2;       /* (9*n^2 - 9*n)/2 */
    return t1 + t2;
}
