/*
 * A048331  Numbers that are repdigits in base 6.
 * https://oeis.org/A048331
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 7*a(n-5) - 6*a(n-10) for n ≥ 10 Initial values: a(0)=0,
 *            a(1)=1, a(2)=2, a(3)=3, a(4)=4, a(5)=5, a(6)=7, a(7)=14,
 *            a(8)=21, a(9)=28 This recurrence generates the numbers that are
 *            repdigits in base 6
 * Range:     u8 n <= 15, u16 n <= 31, u32 n <= 61, u64 n <= 123,
 *            u128 n <= 247, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t init[10] = {
        0, 1, 2, 3, 4,   /* a(0) … a(4) */
        5,                /* a(5)   */
        7, 14, 21, 28     /* a(6)…a(9) */
    };

    if (n < 10) return init[n];

    /* Ring buffer holding the last ten computed values.
       buffer[i] contains a(i) for the current i modulo 10.    */
    seq_t buf[10];
    for (int i = 0; i < 10; ++i) buf[i] = init[i];

    idx_t i;
    for (i = 10; i <= n; ++i) {
        int idx_i      = (int)(i % 10);
        int idx_i5     = (int)((i - 5) % 10);
        int idx_i10    = (int)((i - 10) % 10);
        seq_t val5     = buf[idx_i5];
        seq_t val10    = buf[idx_i10];
        seq_t next     = (seq_t)7 * val5 - (seq_t)6 * val10;
        buf[idx_i] = next;
    }

    return buf[(int)(n % 10)];
}
