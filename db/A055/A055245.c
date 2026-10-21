/*
 * A055245  Numerator sequence of mean length of certain stackings of n+1
 *          squares on a double staircase.
 * https://oeis.org/A055245
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 5*a(n-3) + 3*a(n-5) + a(n-6) initial terms:
 *            a(0)=1, a(1)=1, a(2)=5, a(3)=12, a(4)=28, a(5)=61
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the first six terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)5;
    if (n == 3) return (seq_t)12;
    if (n == 4) return (seq_t)28;
    if (n == 5) return (seq_t)61;

    /* buffer of the last six computed values: indices 0..5 correspond to
       a(n-6), a(n-5), a(n-4), a(n-3), a(n-2), a(n-1) respectively.  */
    seq_t buf[6] = { (seq_t)1, (seq_t)1, (seq_t)5,
                     (seq_t)12, (seq_t)28, (seq_t)61 };
    seq_t value = 0;

    /* iterate from 6 up to n, updating the buffer each step */
    for (idx_t i = 6; i <= n; ++i) {
        value = buf[5] * (seq_t)3
                - buf[3] * (seq_t)5
                + buf[1] * (seq_t)3
                + buf[0];

        /* shift buffer left: discard a(n-6), move others up, store new a(n) */
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = buf[4];
        buf[4] = buf[5];
        buf[5] = value;
    }

    return value;
}
