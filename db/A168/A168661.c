/*
 * A168661  a(n) = n^7*(n^4 + 1)/2.
 * https://oeis.org/A168661
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^7*(n^4 + 1)/2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 7, u64 n <= 60, u128 n <= 3390,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn   = (seq_t)n;
    seq_t n2   = sn * sn;          /* n^2 */
    seq_t n3   = n2 * sn;          /* n^3 */
    seq_t n4   = n2 * n2;          /* n^4 */
    seq_t temp = n4 + (seq_t)1;    /* n^4 + 1 */
    seq_t n7;

    if ((sn & (seq_t)1) == (seq_t)0) {   /* n even → divide n^7 by 2 first */
        seq_t n3_half = n3 / (seq_t)2;
        n7 = n3_half * n4;              /* n^7 / 2 */
    } else {                            /* n odd → divide (n^4+1) by 2 first */
        n7   = n3 * n4;                 /* n^7  */
        temp = temp / (seq_t)2;         /* (n^4 + 1) / 2 */
    }

    return n7 * temp;                    /* final result */
}
