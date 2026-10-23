/*
 * A186878  Number of (n+2)X8 0..3 arrays with every 3X3 subblock commuting
 *          with each horizontal and vertical neighbor 3X3 subblock.
 * https://oeis.org/A186878
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + 3*a(n-3) - 6*a(n-4) + 3*a(n-5) +
 *            3*a(n-6) - 3*a(n-7) for n > 14, with initial values:
 *            a(1)=186096, a(2)=18517, a(3)=32992, a(4)=108640, a(5)=230224,
 *            a(6)=287368, a(7)=338176, a(8)=414184, a(9)=568744,
 *            a(10)=927136, a(11)=1492600, a(12)=2387392, a(13)=4046104,
 *            a(14)=6975931
 * Range:     u8 none, u16 none, u32 n <= 26, u64 n <= 70, u128 n <= 156,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* initial values for n=1..14 */
    static const seq_t init[14] = {
        186096, 18517, 32992, 108640, 230224,
        287368, 338176, 414184, 568744, 927136,
        1492600, 2387392, 4046104, 6975931
    };

    if (n <= 14)
        return init[n - 1];

    /* buffer holds the last 7 terms:
       buf[0] = a(n-7), buf[6] = a(n-1) */
    seq_t buf[7];
    for (idx_t i = 0; i < 7; ++i)
        buf[i] = init[7 + i];              // a(8) .. a(14)

    seq_t next = 0;
    for (idx_t k = 15; k <= n; ++k) {
        /* Compute a(k) using the recurrence */
        next = 2 * buf[6];
        next -= buf[5];
        next += 3 * buf[4];
        next -= 6 * buf[3];
        next += 3 * buf[2];
        next += 3 * buf[1];
        next -= 3 * buf[0];

        /* Shift buffer for next iteration */
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = buf[4];
        buf[4] = buf[5];
        buf[5] = buf[6];
        buf[6] = next;
    }
    return next;
}
