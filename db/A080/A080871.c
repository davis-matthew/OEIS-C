/*
 * A080871  a(n)*a(n+3) - a(n+1)*a(n+2) = 3, given a(0)=a(1)=1, a(2)=4.
 * https://oeis.org/A080871
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=1, a(2)=4, a(3)=7, a(n+4)=8*a(n+2)-a(n)
 *            (equivalently a(n) = (3 + a(n-1)*a(n-2))/a(n-3))
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 43, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 1;
    if (n == 2) return 4;
    if (n == 3) return 7;

    seq_t buf[4];
    buf[0] = 1;
    buf[1] = 1;
    buf[2] = 4;
    buf[3] = 7;
    seq_t eight = 8;

    for (idx_t i = 4; i <= n; ++i) {
        idx_t idx_new   = i & 3;          /* i % 4 */
        idx_t idx_prev2 = (i - 2) & 3;    /* (i-2) % 4 */
        idx_t idx_prev4 = (i - 4) & 3;    /* (i-4) % 4 */
        buf[idx_new] = eight * buf[idx_prev2] - buf[idx_prev4];
    }
    return buf[n & 3];
}
