/*
 * A217758  Triangular numbers of the form k^2 + k - 1.
 * https://oeis.org/A217758
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(2)=55, a(3)=1891; a(n)=35*a(n-1)-35*a(n-2)+a(n-3) for
 *            n>=4
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1u)
        return 1;
    if (n == 2u)
        return 55;
    if (n == 3u)
        return 1891;

    seq_t f1 = 1;      /* a(i-3) */
    seq_t f2 = 55;     /* a(i-2) */
    seq_t f3 = 1891;   /* a(i-1) */
    seq_t fn = 0;

    for (idx_t i = 4; i <= n; ++i) {
        fn = 35 * f3 - 35 * f2 + f1;  /* recurrence */
        f1 = f2;
        f2 = f3;
        f3 = fn;
    }
    return fn;
}
