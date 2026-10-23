/*
 * A165791  Number of tilings of a 4 X n rectangle using dominoes and right
 *          trominoes.
 * https://oeis.org/A165791
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) = 4*a(n-1) + 21*a(n-2) - 25*a(n-3) - 65*a(n-4) - 17*a(n-5)
 *            * + 24*a(n-6) - 11*a(n-7) - 15*a(n-8) + 9*a(n-9) * with a(0)=1,
 *            a(1)=1, a(2)=11, a(3)=55, a(4)=380, a(5)=2319, * a(6)=15171,
 *            a(7)=96139, a(8)=619773
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t init[9] = {
        (seq_t)1,
        (seq_t)1,
        (seq_t)11,
        (seq_t)55,
        (seq_t)380,
        (seq_t)2319,
        (seq_t)15171,
        (seq_t)96139,
        (seq_t)619773
    };

    if (n < 9)
        return init[n];

    /* ring buffer holding the last 9 values a[n-9] .. a[n-1] */
    seq_t buf[9];
    for (idx_t i = 0; i < 9; ++i)
        buf[i] = init[i];

    for (idx_t i = 9; i <= n; ++i) {
        seq_t val = (seq_t)0;
        val += (seq_t)4  * buf[(i - 1) % 9];
        val += (seq_t)21 * buf[(i - 2) % 9];
        val -= (seq_t)25 * buf[(i - 3) % 9];
        val -= (seq_t)65 * buf[(i - 4) % 9];
        val -= (seq_t)17 * buf[(i - 5) % 9];
        val += (seq_t)24 * buf[(i - 6) % 9];
        val -= (seq_t)11 * buf[(i - 7) % 9];
        val -= (seq_t)15 * buf[(i - 8) % 9];
        val += (seq_t)9  * buf[(i - 9) % 9];
        buf[i % 9] = val;
    }

    return buf[n % 9];
}
