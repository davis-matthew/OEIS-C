/*
 * A041351  Denominators of continued fraction convergents to sqrt(189).
 * https://oeis.org/A041351
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 110*a(n-4) - a(n-8) initial values: a(0)=1, a(1)=1,
 *            a(2)=3, a(3)=4, a(4)=107, a(5)=111, a(6)=329, a(7)=440
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 19, u64 n <= 39, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t init[8] = { (seq_t)1, (seq_t)1, (seq_t)3, (seq_t)4,
                                   (seq_t)107, (seq_t)111, (seq_t)329, (seq_t)440 };
    if (n < 8) return init[n];
    seq_t buf[8];
    for (idx_t i = 0; i < 8; ++i) buf[i] = init[i];
    for (idx_t i = 8; i <= n; ++i) {
        seq_t val = (seq_t)110 * buf[(i - 4) % 8] - buf[(i - 8) % 8];
        buf[i % 8] = val;
    }
    return buf[n % 8];
}
