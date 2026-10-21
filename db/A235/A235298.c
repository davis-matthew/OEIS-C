/*
 * A235298  Number of (n+1) X (8+1) 0..3 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 3
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A235298
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 17*a(n-2) - 57*a(n-3) - 86*a(n-4) + 372*a(n-5)
 *            * + 76*a(n-6) - 972*a(n-7) + 360*a(n-8) + 864*a(n-9) -
 *            576*a(n-10) * Empirical recurrence; initial values: *
 *            a(1)=37536, a(2)=42100, a(3)=48128, a(4)=60856, a(5)=79728, *
 *            a(6)=120112, a(7)=187184, a(8)=333952, a(9)=603216, a(10)=121...
 * Range:     u8 none, u16 n <= 4, u32 n <= 19, u64 n <= 40, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t init[10] = {
        (seq_t)37536,  (seq_t)42100,  (seq_t)48128,
        (seq_t)60856,  (seq_t)79728,  (seq_t)120112,
        (seq_t)187184, (seq_t)333952, (seq_t)603216,
        (seq_t)1215520
    };

    if (n <= 10) return init[n - 1];

    seq_t buf[10];
    for (int i = 0; i < 10; ++i) buf[i] = init[i];

    for (idx_t i = 11; ; ++i) {
        seq_t t1 = (seq_t)3   * buf[9];
        seq_t t2 = (seq_t)17  * buf[8];
        seq_t t3 = (seq_t)57  * buf[7];
        seq_t t4 = (seq_t)86  * buf[6];
        seq_t t5 = (seq_t)372 * buf[5];
        seq_t t6 = (seq_t)76  * buf[4];
        seq_t t7 = (seq_t)972 * buf[3];
        seq_t t8 = (seq_t)360 * buf[2];
        seq_t t9 = (seq_t)864 * buf[1];
        seq_t t10 = (seq_t)576 * buf[0];

        seq_t next = t1 + t2 - t3 - t4 + t5 + t6 - t7 + t8 + t9 - t10;

        /* shift buffer */
        for (int k = 0; k < 9; ++k) buf[k] = buf[k + 1];
        buf[9] = next;

        if (i == n) return next;
    }
}
