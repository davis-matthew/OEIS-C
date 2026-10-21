/*
 * A077248  Combined Diophantine Chebyshev sequences A077246 and A077244.
 * https://oeis.org/A077248
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-2) - a(n-4) with initial values a(0)=2, a(1)=3,
 *            a(2)=13, a(3)=22
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 43, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle small indices directly */
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)13;
    if (n == 3) return (seq_t)22;

    /* circular buffer for the last four terms */
    seq_t t[4];
    t[0] = (seq_t)2;   /* a(0) */
    t[1] = (seq_t)3;   /* a(1) */
    t[2] = (seq_t)13;  /* a(2) */
    t[3] = (seq_t)22;  /* a(3) */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t ai = (seq_t)8 * t[(i - 2) & 3] - t[(i - 4) & 3];
        t[i & 3] = ai;
    }
    return t[n & 3];
}
