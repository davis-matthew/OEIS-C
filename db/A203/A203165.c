/*
 * A203165  (n-1)-st elementary symmetric function of the first n terms of the
 *          periodic sequence (3,4,1,2,3,4,1,2,...).
 * https://oeis.org/A203165
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) * s_n + P_{n-1}, a(1)=1, where s_n is the n-th
 *            term of the periodic sequence (3,4,1,2,3,4,1,2,...). P_n =
 *            product of the first n terms. This computes the (n-1)-st
 *            elementary symmetric sum of the first n terms of that sequence
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 24, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* External linkage required */
seq_t a(idx_t n)
{
    /* product of terms seen so far: P_0 = 1, a_0 = 0 */
    seq_t P = 1;
    seq_t a_val = 0;

    /* Periodic values of the sequence */
    static const unsigned char period[4] = {3, 4, 1, 2};

    for (idx_t i = 1; i <= n; ++i)
    {
        unsigned char s = period[(i - 1) & 3];           /* s_i */
        a_val = a_val * (seq_t)s + P;                   /* a_i = a_{i-1}*s + P_{i-1} */
        P *= (seq_t)s;                                  /* update product */
    }
    return a_val;
}
