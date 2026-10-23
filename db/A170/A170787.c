/*
 * A170787  a(n) = n^9*(n^5 + 1)/2.
 * https://oeis.org/A170787
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^9*(n^5 + 1)/2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 24, u128 n <= 593,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;

    /* compute powers of n */
    seq_t n2 = n_seq * n_seq;        /* n^2 */
    seq_t n3 = n2 * n_seq;           /* n^3 */
    seq_t n4 = n2 * n2;              /* n^4 */
    seq_t n5 = n3 * n2;              /* n^5 */
    seq_t n9 = n5 * n4;              /* n^9 */

    if ((n_seq & (seq_t)1) == (seq_t)0) {          /* n even */
        seq_t half_n9 = n9 / (seq_t)2;            /* divide n^9 by 2 */
        return half_n9 * (n5 + (seq_t)1);
    } else {                                      /* n odd */
        seq_t half_n5p1 = (n5 + (seq_t)1) / (seq_t)2; /* divide (n^5+1) by 2 */
        return n9 * half_n5p1;
    }
}
