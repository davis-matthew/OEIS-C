/*
 * A190528  Number of n-step one-sided prudent walks avoiding exactly three
 *          consecutive West steps.
 * https://oeis.org/A190528
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk, easy
 * Formula:   a(n) = 2*a(n-1) + a(n-2) - a(n-4) + a(n-5) for n≥5 Initial
 *            values: a(0)=1, a(1)=3, a(2)=7, a(3)=16, a(4)=39
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 25, u64 n <= 50, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 3;
    if (n == 2) return 7;
    if (n == 3) return 16;
    if (n == 4) return 39;

    seq_t a0 = 1;   /* a(n-5) */
    seq_t a1 = 3;   /* a(n-4) */
    seq_t a2 = 7;   /* a(n-3) */
    seq_t a3 = 16;  /* a(n-2) */
    seq_t a4 = 39;  /* a(n-1) */
    seq_t cur = 0;

    for (idx_t i = 5; i <= n; ++i) {
        cur = (2 * a4 + a3 + a0) - a1;  /* a(n) = 2*a(n-1)+a(n-2)-a(n-4)+a(n-5) */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = cur;
    }
    return cur;
}
