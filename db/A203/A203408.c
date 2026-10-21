/*
 * A203408  Numbers which are both heptagonal and decagonal.
 * https://oeis.org/A203408
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(1)=1, a(2)=540, a(3)=2887450, a(4)=1123674201;
 * a(n) = 2079362*a(n−2) − a(n−4) + 818748   for n ≥ 5
 */

seq_t a(idx_t n)
{
    /* handle the first four terms directly */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)540;
    if (n == 3) return (seq_t)2887450;
    if (n == 4) return (seq_t)1123674201;

    /* buf[0] = a(i−4), buf[1] = a(i−3), buf[2] = a(i−2), buf[3] = a(i−1) */
    seq_t buf[4];
    buf[0] = (seq_t)1;
    buf[1] = (seq_t)540;
    buf[2] = (seq_t)2887450;
    buf[3] = (seq_t)1123674201;

    for (idx_t i = 5; i <= n; ++i) {
        /* a(i) = 2079362*a(i−2) – a(i−4) + 818748 */
        seq_t cur = (seq_t)2079362 * buf[2] - buf[0] + (seq_t)818748;

        /* shift the buffer for the next iteration */
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = cur;
    }

    return buf[3];
}
