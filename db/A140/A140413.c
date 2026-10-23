/*
 * A140413  a(2n) = A000045(6n) + 1, a(2n+1) = A000045(6n+3) - 1.
 * https://oeis.org/A140413
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=1, a(2)=9; For n>=3: a(n)=3*a(n-1)+5*a(n-2)+a(n-3)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 31, u128 n <= 62,
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
        return (seq_t)1;
    if (n == 2)
        return (seq_t)9;

    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)1;   /* a(1) */
    seq_t a2 = (seq_t)9;   /* a(2) */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t ai = a2 * (seq_t)3 + a1 * (seq_t)5 + a0;
        a0 = a1;
        a1 = a2;
        a2 = ai;
    }

    return a2;
}
