/*
 * A152740  11 times triangular numbers.
 * https://oeis.org/A152740
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11*n*(n+1)/2
 * Range:     u8 n <= 6, u16 n <= 108, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;              /* promote index to seq_t */
    seq_t t  = nn * (nn + 1) / 2;     /* triangular number */
    return 11 * t;                    /* scale by 11 */
}
