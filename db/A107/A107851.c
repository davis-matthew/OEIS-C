/*
 * A107851  Expansion of g.f.
 *          x*(-1-x-3*x^2-x^3+2*x^5)/((2*x^3+x^2-1)*(x^4+1)).
 * https://oeis.org/A107851
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-2) + 2*a(n-3) - a(n-4) + a(n-6) + 2*a(n-7), a(0)=0,
 *            a(1)=1, a(2)=1, a(3)=4, a(4)=4, a(5)=5, a(6)=9, a(7)=10
 * Range:     u8 n <= 14, u16 n <= 27, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* initial terms for n = 0..7 */
    static const seq_t init[8] = {0,1,1,4,4,5,9,10};

    /* return initial values directly */
    if (n < 8)
        return init[n];

    /* R[0] = a(n-1), R[1] = a(n-2), ..., R[6] = a(n-7) for the current n */
    seq_t R[7];
    R[0] = init[7]; /* a7  */
    R[1] = init[6]; /* a6  */
    R[2] = init[5]; /* a5  */
    R[3] = init[4]; /* a4  */
    R[4] = init[3]; /* a3  */
    R[5] = init[2]; /* a2  */
    R[6] = init[1]; /* a1  */

    seq_t an = 0;
    for (idx_t i = 8; i <= n; ++i)
    {
        /* compute a(i) = R[1] + 2*R[2] - R[3] + R[5] + 2*R[6] */
        seq_t sum = R[1] + 2*R[2] + R[5] + 2*R[6];
        an = sum - R[3];

        /* shift the ring buffer for the next iteration */
        R[6] = R[5];
        R[5] = R[4];
        R[4] = R[3];
        R[3] = R[2];
        R[2] = R[1];
        R[1] = R[0];
        R[0] = an;
    }
    return an;
}
