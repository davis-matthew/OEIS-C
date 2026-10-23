/*
 * A178633  a(n) = 54*((10^n - 1)/9)^2.
 * https://oeis.org/A178633
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 111*a(n-1) - 1110*a(n-2) + 1000*a(n-3) for n>3 with
 *            initial values a(1)=54, a(2)=6534, a(3)=665334
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first three terms directly */
    if (n == 1) return 54;
    if (n == 2) return 6534;
    if (n == 3) return 665334;

    /* Use a sliding window of the last three computed terms */
    seq_t t1 = 54;          /* a(1) */
    seq_t t2 = 6534;        /* a(2) */
    seq_t t3 = 665334;      /* a(3) */
    seq_t tn = 0;

    for (idx_t i = 4; i <= n; ++i) {
        tn = 111 * t3 - 1110 * t2 + 1000 * t1;   /* recurrence */
        t1 = t2;
        t2 = t3;
        t3 = tn;
    }
    return tn;
}
