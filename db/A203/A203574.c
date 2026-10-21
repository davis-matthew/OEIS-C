/*
 * A203574  Bisection of A099924 (Lucas convolution); one half of the terms
 *          with odd arguments.
 * https://oeis.org/A203574
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 19, u64 n <= 41, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A203574: Bisection of A099924 (Lucas convolution)
 * a(n) = a(n-1) + a(n-2) + a(n-3) + a(n-4) with signs: a(n) = 6*a(n-1) - 11*a(n-2) + 6*a(n-3) - a(n-4)
 * Initial values: a(0)=2, a(1)=11, a(2)=41, a(3)=137
 * The recurrence is used to compute the terms efficiently.
 */
seq_t a(idx_t n)
{
    /* Base values for n = 0,1,2,3 */
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)11;
    if (n == 2) return (seq_t)41;
    if (n == 3) return (seq_t)137;

    /* Buffer to hold the four most recent values:
       buf[0] = a(k-4), buf[1] = a(k-3), buf[2] = a(k-2), buf[3] = a(k-1)  */
    seq_t buf[4];
    buf[0] = (seq_t)2;
    buf[1] = (seq_t)11;
    buf[2] = (seq_t)41;
    buf[3] = (seq_t)137;

    /* Iterate from k = 4 up to the requested n */
    for (idx_t k = 4; k <= n; ++k) {
        /* Indices modulo 4 for the needed terms */
        idx_t i_k1 = (k - 1) & 3;   /* a(k-1) */
        idx_t i_k2 = (k - 2) & 3;   /* a(k-2) */
        idx_t i_k3 = (k - 3) & 3;   /* a(k-3) */
        idx_t i_k4 = (k - 4) & 3;   /* a(k-4) */

        /* Apply the recurrence:
           a(k) = 6*a(k-1) - 11*a(k-2) + 6*a(k-3) - a(k-4)            */
        seq_t term1 = (seq_t)6  * buf[i_k1];
        seq_t term2 = (seq_t)6  * buf[i_k3];
        seq_t term4 = (seq_t)11 * buf[i_k2];

        seq_t ak = term1 + term2;  /* 6*a(k-1) + 6*a(k-3) */
        ak -= term4;              /* -11*a(k-2) */
        ak -= buf[i_k4];           /* -a(k-4) */

        /* Store the new value */
        buf[k & 3] = ak;
    }

    return buf[n & 3];
}
