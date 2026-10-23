/*
 * A041043  Denominators of continued fraction convergents to sqrt(27).
 * https://oeis.org/A041043
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 52*a(n-2) - a(n-4), a(0)=1, a(1)=5, a(2)=51, a(3)=260
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the first four terms explicitly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;
    if (n == 2) return (seq_t)51;
    if (n == 3) return (seq_t)260;

    /* a[0]..a[3] circulate the last four computed values */
    seq_t a[4] = { (seq_t)1, (seq_t)5, (seq_t)51, (seq_t)260 };

    for (idx_t i = 4; i <= n; ++i) {
        seq_t cur = (seq_t)52 * a[(i - 2) & 3] - a[i & 3];
        a[i & 3] = cur;                /* overwrite the oldest value */
    }

    return a[n & 3];
}
