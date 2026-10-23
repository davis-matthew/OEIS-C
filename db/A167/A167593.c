/*
 * A167593  The third left hand column of triangle A167591.
 * https://oeis.org/A167593
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 12*a(n-1) - 60*a(n-2) + 160*a(n-3) - 240*a(n-4) +
 *            192*a(n-5) - 64*a(n-6) Initial values (offset 3): a(3)=9,
 *            a(4)=120, a(5)=952, a(6)=5664, a(7)=27888, a(8)=120064,
 *            a(9)=467712, a(10)=1686528
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 17, u64 n <= 42, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the defined offset: a(n) only for n >= 3 */
    if (n < 3)
        return 0;

    /* Initial terms up to a(8) */
    static const seq_t base[9] = {
        0, 0, 0,          /* indices 0‑2 unused */
        9,          /* a(3) */
        120,        /* a(4) */
        952,        /* a(5) */
        5664,       /* a(6) */
        27888,      /* a(7) */
        120064      /* a(8) */
    };

    if (n <= 8)
        return base[n];

    /* Use a sliding window of the previous six values.
       buf[0] = a(k-6), buf[1] = a(k-5), ..., buf[5] = a(k-1)   */
    seq_t buf[6] = {
        base[3],   /* a(3) = a(k-6) when k = 9 */
        base[4],   /* a(4) = a(k-5) */
        base[5],   /* a(5) = a(k-4) */
        base[6],   /* a(6) = a(k-3) */
        base[7],   /* a(7) = a(k-2) */
        base[8]    /* a(8) = a(k-1) */
    };

    seq_t current = 0;
    for (idx_t k = 9; k <= n; ++k) {
        current = 12 * buf[5]
                - 60 * buf[4]
                + 160 * buf[3]
                - 240 * buf[2]
                + 192 * buf[1]
                - 64  * buf[0];

        /* shift window for next iteration */
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = buf[4];
        buf[4] = buf[5];
        buf[5] = current;
    }
    return current;
}
