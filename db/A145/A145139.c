/*
 * A145139  Antidiagonal sums of A145153.
 * https://oeis.org/A145139
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 2*a(n-2) + a(n-4) - 2*a(n-5) for n>=5 with
 *            initial values a(0)=0, a(1)=1, a(2)=1, a(3)=2, a(4)=4
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1 || n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)2;
    if (n == 4) return (seq_t)4;

    seq_t a0 = (seq_t)0;  /* a[n-5] */
    seq_t a1 = (seq_t)1;  /* a[n-4] */
    seq_t a2 = (seq_t)1;  /* a[n-3] */
    seq_t a3 = (seq_t)2;  /* a[n-2] */
    seq_t a4 = (seq_t)4;  /* a[n-1] */
    seq_t an = 0;

    for (idx_t i = 5; i <= n; ++i) {
        an = (seq_t)3 * a4 - (seq_t)2 * a3 + a1 - (seq_t)2 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = an;
    }
    return an;
}
