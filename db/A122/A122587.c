/*
 * A122587  Leading digit of n in base 4.
 * https://oeis.org/A122587
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = floor(n/(4^floor(log_4(n))))
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;           /* undefined for offset=1, but safe guard */
    }

    seq_t sn = (seq_t)n;      /* convert index to computing type */
    seq_t pow4 = 1;          /* 4^k, starting with k=0 */

    /* increase pow4 while the next 4-times value would not exceed sn */
    while (pow4 <= sn / 4) {
        pow4 *= 4;
    }

    return sn / pow4;        /* leading digit in base 4 */
}
