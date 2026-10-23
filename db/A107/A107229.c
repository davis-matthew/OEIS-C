/*
 * A107229  a(n) = 2*a(n-2)+5*a(n-3), n>6.
 * https://oeis.org/A107229
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-2) + 5*a(n-3) (for n ≥ 8) initial values: a(0)=1,
 *            a(1)=1, a(2)=1, a(3)=3, a(4)=3, a(5)=11, a(6)=9, a(7)=7
 * Range:     u8 n <= 10, u16 n <= 17, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t init[8] = {
        (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)3,
        (seq_t)3, (seq_t)11, (seq_t)9, (seq_t)7
    };

    if (n < 8)
        return init[n];

    /* use a small circular buffer of the last 8 terms */
    seq_t buf[8];
    for (idx_t i = 0; i < 8; ++i)
        buf[i] = init[i];

    for (idx_t i = 8; i <= n; ++i) {
        /* compute a(i) from a(i-2) and a(i-3) */
        buf[i % 8] = (seq_t)2 * buf[(i - 2) % 8] + (seq_t)5 * buf[(i - 3) % 8];
    }

    return buf[n % 8];
}
