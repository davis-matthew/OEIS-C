/*
 * A156096  Inverse binomial transform of A030186.
 * https://oeis.org/A156096
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-2) + 2*a(n-3) with a(0)=1, a(1)=1, a(2)=4
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 28, u64 n <= 56, u128 n <= 112,
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

    seq_t x = 1;    /* a(n-3) */
    seq_t y = 1;    /* a(n-2) */
    seq_t z = 4;    /* a(n-1) */
    seq_t cur;

    for (idx_t i = 3; i <= n; ++i) {
        cur = y * 4 + x * 2; /* a(i) = 4*a(i-2) + 2*a(i-3) */
        x = y;
        y = z;
        z = cur;
    }
    return z;
}
