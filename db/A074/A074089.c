/*
 * A074089  Coefficient of q^3 in nu(n), where nu(0)=1, nu(1)=b and, for n >=
 *          2, nu(n) = b*nu(n-1) + lambda*(1+q+q^2+...+q^(n-2))*nu(n-2) with
 *          (b,lambda)=(2,3).
 * https://oeis.org/A074089
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) satisfies the 8‑term linear recurrence a(n) = 8⋅a(n-1)
 *            –12⋅a(n-2) –40⋅a(n-3) +74⋅a(n-4) +120⋅a(n-5) –108⋅a(n-6)
 *            –216⋅a(n-7) –81⋅a(n-8) for n ≥ 11, with initial values a(0)=0,
 *            a(1)=0, a(2)=0, a(3)=0, a(4)=0, a(5)=78, a(6)=501, a(7)=2574,
 *            a(8)=11757, a(9)=50034, a(10)=203229
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 36, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Initial values up to n = 10 */
    static const seq_t init[11] = {
        (seq_t)0,
        (seq_t)0,
        (seq_t)0,
        (seq_t)0,
        (seq_t)0,
        (seq_t)78,
        (seq_t)501,
        (seq_t)2574,
        (seq_t)11757,
        (seq_t)50034,
        (seq_t)203229
    };

    if (n <= 10)
        return init[n];

    /* Circular buffer holding a[i-8] .. a[i-1] */
    seq_t buf[8] = {
        init[3],  /* a(3)  */
        init[4],  /* a(4)  */
        init[5],  /* a(5)  */
        init[6],  /* a(6)  */
        init[7],  /* a(7)  */
        init[8],  /* a(8)  */
        init[9],  /* a(9)  */
        init[10]  /* a(10) */
    };

    seq_t next;
    for (idx_t i = 11; i <= n; ++i) {
        /* Positive contributions */
        seq_t pos = 8 * buf[7]          /* a(i-1) */
                + 74 * buf[4]         /* a(i-4) */
                + 120 * buf[3];       /* a(i-5) */

        /* Negative contributions */
        seq_t neg = 12 * buf[6]         /* a(i-2) */
                + 40 * buf[5]         /* a(i-3) */
                + 108 * buf[2]        /* a(i-6) */
                + 216 * buf[1]        /* a(i-7) */
                + 81 * buf[0];        /* a(i-8) */

        next = pos - neg;  /* result is always non‑negative */

        /* Shift the circular buffer: remove oldest, add newest */
        for (int k = 0; k < 7; ++k)
            buf[k] = buf[k + 1];
        buf[7] = next;
    }

    return buf[7];
}
