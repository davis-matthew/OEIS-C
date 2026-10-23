/*
 * A027138  a(n) = Sum_{k=0..2n} (k+1) * A027113(n, 2n-k).
 * https://oeis.org/A027138
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 7*a(n-2) + 3*a(n-3) (n > 4) Initial values:
 *            a(1) = 1, a(2) = 7, a(3) = 26, a(4) = 85
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* offset is 1 – not used by callers */
    if (n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)7;
    if (n == 3)
        return (seq_t)26;
    if (n == 4)
        return (seq_t)85;

    seq_t a1 = (seq_t)1;   /* a(n-3) when n == 5 */
    seq_t a2 = (seq_t)7;   /* a(n-2) when n == 5 */
    seq_t a3 = (seq_t)26;  /* a(n-1) when n == 5 */
    seq_t a4 = (seq_t)85;  /* a(n) at n==4, becomes a(n-1) in loop */

    for (idx_t i = 5; i <= n; ++i) {
        seq_t cur = ((seq_t)5) * a4 - ((seq_t)7) * a3 + ((seq_t)3) * a2;
        a2 = a3;
        a3 = a4;
        a4 = cur;
    }
    return a4;
}
