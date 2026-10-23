/*
 * A168220  a(n) = n^4*(n^6 + 1)/2.
 * https://oeis.org/A168220
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^4 * (n^6 + 1) / 2
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 90, u128 n <= 7643,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute n^4 * (n^6 + 1) / 2 without intermediate overflow
 * For even n: divide n^4 by 2 before the multiplication.
 * For odd n: divide (n^6 + 1) by 2 before the multiplication. */
seq_t a(idx_t n)
{
    seq_t m = (seq_t)n;               /* promote index to seq_t */

    /* n^2 */
    seq_t n2 = m * m;
    /* n^4 */
    seq_t n4 = n2 * n2;
    /* n^6 = n^4 * n^2 */
    seq_t n6 = n4 * n2;

    if ((n & 1) == 0) { /* n even */
        /* n^4 is even, halve it first */
        seq_t n4_half = n4 / 2;
        return n4_half * (n6 + 1);
    } else {            /* n odd */
        /* (n^6 + 1) is even, halve it first */
        seq_t term_half = (n6 + 1) / 2;
        return n4 * term_half;
    }
}
