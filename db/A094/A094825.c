/*
 * A094825  Number of (s(0), s(1), ..., s(2n)) such that 0 < s(i) < 10 and
 *          |s(i) - s(i-1)| = 1 for i = 1,2,...,2n, s(0) = 1, s(2n) = 7.
 * https://oeis.org/A094825
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) - 21*a(n-2) + 20*a(n-3) - 5*a(n-4) with a(3)=1,
 *            a(4)=7, a(5)=35, a(6)=153
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 19, u64 n <= 36, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3) return 0;            /* below offset – result irrelevant */

    if (n == 3) return 1;
    if (n == 4) return 7;
    if (n == 5) return 35;
    if (n == 6) return 153;

    seq_t a3 = 1, a4 = 7, a5 = 35, a6 = 153;
    idx_t i;
    for (i = 7; i <= n; ++i) {
        seq_t curr = a6 * 8 - a5 * 21 + a4 * 20 - a3 * 5;
        a3 = a4;
        a4 = a5;
        a5 = a6;
        a6 = curr;
    }
    return a6;
}
