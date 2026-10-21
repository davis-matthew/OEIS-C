/*
 * A099783  a(n) = Sum_{k=0..floor(n/3)} C(n-k,2*k)*3^(n-2*k).
 * https://oeis.org/A099783
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 6*a(n-1) - 9*a(n-2) + 3*a(n-3) initial values: a(0)=1,
 *            a(1)=3, a(2)=9
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 33, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)3;
    if (n == 2)
        return (seq_t)9;

    seq_t a0 = (seq_t)1;   /* a(n-3) */
    seq_t a1 = (seq_t)3;   /* a(n-2) */
    seq_t a2 = (seq_t)9;   /* a(n-1) */
    seq_t ai = 0;

    for (idx_t i = 3; i <= n; ++i) {
        ai = (seq_t)6 * a2;           /* 6*a(n-1) */
        ai -= (seq_t)9 * a1;          /* -9*a(n-2) */
        ai += (seq_t)3 * a0;          /* +3*a(n-3) */
        a0 = a1;                      /* shift window */
        a1 = a2;
        a2 = ai;
    }
    return a2;
}
