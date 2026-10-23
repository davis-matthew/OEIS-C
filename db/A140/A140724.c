/*
 * A140724  Period 10: 1, 5, 9, 7, 7, 9, 5, 1, 3, 3 repeated.
 * https://oeis.org/A140724
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1)-2*a(n-2)+2*a(n-3)-2*a(n-4)+a(n-5); Initial
 *            values: a(0)=1, a(1)=5, a(2)=9, a(3)=7, a(4)=7
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Initial 5 terms */
    static const seq_t init[5] = { (seq_t)1, (seq_t)5, (seq_t)9, (seq_t)7, (seq_t)7 };
    if (n < 5)
        return init[n];

    /* Working window of the last five terms */
    seq_t f[5] = { init[0], init[1], init[2], init[3], init[4] };

    for (idx_t i = 5; i <= n; ++i)
    {
        /* Use signed arithmetic on small integers to avoid unsigned wrap */
        int t = 2 * (int)f[4]
              - 2 * (int)f[3]
              + 2 * (int)f[2]
              - 2 * (int)f[1]
              + (int)f[0];

        /* Advance the window */
        f[0] = f[1];
        f[1] = f[2];
        f[2] = f[3];
        f[3] = f[4];
        f[4] = (seq_t)t;
    }

    return f[4];
}
