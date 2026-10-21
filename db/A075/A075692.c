/*
 * A075692  Upper irredundance number for queens graph Q_n on n^2 nodes.
 * https://oeis.org/A075692
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 1 for n=1 or 2; 2 for n=3; 4 for n=4; 2n-5 for n≥5
 * Range:     u8 n <= 10, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 2)
        return (seq_t)1;
    if (n == 3)
        return (seq_t)2;
    if (n == 4)
        return (seq_t)4;
    return (seq_t)2 * (seq_t)n - (seq_t)5;
}
