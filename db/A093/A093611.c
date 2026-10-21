/*
 * A093611  Numerators of convergents to 3/(1 + sqrt(10)).
 * https://oeis.org/A093611
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = 6*a(n-3) + a(n-6) for n >= 8 initial terms: a(1)=0,
 *            a(2)=1, a(3)=3, a(4)=4, a(5)=7, a(6)=18, a(7)=25
 * Range:     u8 n <= 10, u16 n <= 20, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the offset: 1 based indexing */
    if (n <= 7) {
        static const seq_t init[7] = {0, 1, 3, 4, 7, 18, 25};
        return init[n - 1];
    }

    /* buffer holds the last seven values: a(k) is stored at index (k-1) % 7 */
    seq_t buf[7];
    buf[0] = 0;   /* a(1) */
    buf[1] = 1;   /* a(2) */
    buf[2] = 3;   /* a(3) */
    buf[3] = 4;   /* a(4) */
    buf[4] = 7;   /* a(5) */
    buf[5] = 18;  /* a(6) */
    buf[6] = 25;  /* a(7) */

    for (idx_t i = 8; i <= n; ++i) {
        /* a(n-3) is at index (i-4) % 7, a(n-6) at (i-7) % 7 */
        seq_t next = (seq_t)6 * buf[(i - 4) % 7] + buf[(i - 7) % 7];
        buf[(i - 1) % 7] = next;   /* store a(i) */
    }
    return buf[(n - 1) % 7];
}
