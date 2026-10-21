/*
 * A227122  Number of n X 3 0,1 arrays indicating 2 X 2 subblocks of some
 *          larger (n+1) X 4 binary array having a sum of zero, with rows and
 *          columns of the latter in lexicographically nondecreasing order.
 * https://oeis.org/A227122
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n-1) - 45*a(n-2) + 120*a(n-3) - 210*a(n-4) +
 *            252*a(n-5) - 210*a(n-6) + 120*a(n-7) - 45*a(n-8) + 10*a(n-9) -
 *            a(n-10), for n > 12 Initial values: a(1)=4, a(2)=13, a(3)=33,
 *            a(4)=81, a(5)=202, a(6)=492, a(7)=1143, a(8)=2524, a(9)=5315,
 *            a(10)=10718, a(11)=20776, a(12)=38839
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 48, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t init[12] = {4,13,33,81,202,492,1143,2524,5315,10718,20776,38839};

    if (n <= 12 && n >= 1)
        return init[n-1];

    /* buffer holding the last 10 values: a(n-1) .. a(n-10) */
    seq_t buf[10];
    /* initialize buf with a(12) down to a(3) */
    for (idx_t i = 0; i < 10; ++i)
        buf[i] = init[12 - i - 1];

    for (idx_t k = 13; k <= n; ++k)
    {
        seq_t pos = buf[0] * 10 +          /* a(n-1) * 10 */
                    buf[2] * 120 +         /* a(n-3) * 120 */
                    buf[4] * 252 +         /* a(n-5) * 252 */
                    buf[6] * 120 +         /* a(n-7) * 120 */
                    buf[8] * 10;           /* a(n-9) * 10 */

        seq_t neg = buf[1] * 45  +         /* a(n-2) * 45  */
                    buf[3] * 210 +        /* a(n-4) * 210 */
                    buf[5] * 210 +        /* a(n-6) * 210 */
                    buf[7] * 45  +        /* a(n-8) * 45  */
                    buf[9];               /* a(n-10)      */

        seq_t next = pos - neg;

        /* shift buffer right, inserting new value at front */
        for (int j = 9; j >= 1; --j)
            buf[j] = buf[j-1];
        buf[0] = next;
    }

    return buf[0];
}
