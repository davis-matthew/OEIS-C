/*
 * A225126  Central terms of the triangle in A048152.
 * https://oeis.org/A225126
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=0; a(2n)=n; a(2n+1)=3n+1
 * Range:     u8 n <= 170, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)0;

    seq_t sn = (seq_t)n;
    if ((n & 1) == 0) {            /* n is even: n = 2*m */
        return sn / 2;
    } else {                        /* n is odd: n = 2*m + 1,  m >= 1 */
        seq_t m = (sn - 1) / 2;    /* Compute m = (n-1)/2 */
        return 3 * m + 1;
    }
}
