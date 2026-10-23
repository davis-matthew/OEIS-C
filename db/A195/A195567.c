/*
 * A195567  Hypotenuses of primitive Pythagorean triples in A195565 and
 *          A195566.
 * https://oeis.org/A195567
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies a(n) = 4098*a(n-5) - a(n-10) for n > 10, with
 *            initial terms
 *            a(1)=5,a(2)=5,a(3)=325,a(4)=449,a(5)=3649,a(6)=20165,
 *            a(7)=20485,a(8)=1331845,a(9)=1840001,a(10)=14953601
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 12, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const seq_t init[10] = {5, 5, 325, 449, 3649,
                                   20165, 20485, 1331845, 1840001, 14953601};

    if (n <= 10)
        return init[n - 1];

    seq_t buf[10];
    for (int i = 0; i < 10; ++i)
        buf[i] = init[9 - i];      /* buf[0]=a(10), buf[9]=a(1) */

    for (idx_t k = 11; k <= n; ++k) {
        seq_t new_val = 4098 * buf[4] - buf[9];
        for (int i = 9; i > 0; --i)
            buf[i] = buf[i - 1];
        buf[0] = new_val;
    }
    return buf[0];
}
