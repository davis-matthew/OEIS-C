/*
 * A060827  3-wave sequence beginning with 2's.
 * https://oeis.org/A060827
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=2, a(1)=2, a(2)=2, a(3)=4, a(4)=6, a(5)=10; for n>=6: a(n)
 *            = a(n-4) + 2*a(n-2) - a(n-6) (Sequence A060827 = 2*A038196, with
 *            generating function (-2x^2+2x+2)/(x^6-x^4-2x^2+1).)
 * Range:     u8 n <= 13, u16 n <= 26, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t init[6] = {2, 2, 2, 4, 6, 10};
    if (n < 6) return init[n];

    seq_t buf[6] = {init[0], init[1], init[2], init[3], init[4], init[5]};
    for (idx_t i = 6; i <= n; ++i) {
        seq_t dbl = buf[4] + buf[4];          // 2*a(i-2)
        seq_t next = buf[2] + dbl - buf[0];   // a(i-4) + 2*a(i-2) - a(i-6)
        /* shift buffer */
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = buf[4];
        buf[4] = buf[5];
        buf[5] = next;
    }
    return buf[5];
}
