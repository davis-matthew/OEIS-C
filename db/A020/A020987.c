/*
 * A020987  Zero-one version of Golay-Rudin-Shapiro sequence (or word).
 * https://oeis.org/A020987
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = (1 - R(n)) / 2 where R(n) is the Rudin–Shapiro +1/-1
 *            sequence: R(0)=1, R(2k)=R(k), R(2k+1)=(-1)^k R(k)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    int rs = 1;                 /* current value of R(n) (+1 or -1)   */
    idx_t m = n;

    while (m != 0) {
        if ((m & 1) != 0) {     /* odd index: apply (-1)^k factor   */
            /* k = m >> 1; parity of k determines sign */
            if (((m >> 1) & 1) != 0)
                rs = -rs;
        }
        m >>= 1;                /* move to n/2 in the recurrence    */
    }

    return (seq_t)((1 - rs) / 2);   /* zero‑one conversion            */
}
