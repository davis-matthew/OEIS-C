/*
 * A041346  Numerators of continued fraction convergents to sqrt(187).
 * https://oeis.org/A041346
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 3364*a(n-6) - a(n-12), a(0)=13, a(1)=14, a(2)=41,
 *            a(3)=547, a(4)=1135, a(5)=1682, a(6)=44867, a(7)=46549,
 *            a(8)=137965, a(9)=1840094, a(10)=3818153, a(11)=5658247
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 14, u64 n <= 32, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t init[12] = {
        13, 14, 41, 547, 1135, 1682,
        44867, 46549, 137965, 1840094, 3818153, 5658247
    };
    if (n < 12) return init[n];

    seq_t buf[12];
    for (idx_t i = 0; i < 12; ++i) buf[i] = init[i];

    for (idx_t i = 12; i <= n; ++i) {
        seq_t val = (seq_t)3364 * buf[(i - 6) % 12] - buf[(i - 12) % 12];
        buf[i % 12] = val;
    }
    return buf[n % 12];
}
