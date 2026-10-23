/*
 * A010004  a(0) = 1, a(n) = 13*n^2 + 2 for n>0.
 * https://oeis.org/A010004
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if n == 0; otherwise a(n) = 13*n^2 + 2
 * Range:     u8 n <= 4, u16 n <= 71, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u) {
        return (seq_t)1;
    }
    /* Promote index n to seq_t before any arithmetic */
    seq_t sn = (seq_t)n;
    /* Compute 13 * n^2 + 2 using seq_t arithmetic only */
    seq_t val = (seq_t)13 * sn * sn + (seq_t)2;
    return val;
}
