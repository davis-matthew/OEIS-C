/*
 * A089931  a(n) = 3*a(n-1) + 3*a(n-3) + a(n-4).
 * https://oeis.org/A089931
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*a(n-1) + 3*a(n-3) + a(n-4) (n ≥ 4) Initial values:
 *            a(0)=1, a(1)=3, a(2)=9, a(3)=30
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 37, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the initial terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)9;
    if (n == 3) return (seq_t)30;

    /* Rolling variables for a[i-4], a[i-3], a[i-2], a[i-1] */
    seq_t a0 = (seq_t)1;   /* a(n-4) */
    seq_t a1 = (seq_t)3;   /* a(n-3) */
    seq_t a2 = (seq_t)9;   /* a(n-2) */
    seq_t a3 = (seq_t)30;  /* a(n-1) */

    for (idx_t i = 4; i <= n; ++i)
    {
        seq_t an = (a3 * (seq_t)3) + (a1 * (seq_t)3) + a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return a3;
}
