/*
 * A021184  Expansion of 1/((1-x)*(1-2*x)*(1-6*x)*(1-8*x)).
 * https://oeis.org/A021184
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*8^(n+3) - 21*6^(n+3) + 35*2^(n+3) - 24)/840
 *            Recurrence: a(0) = 1, a(1) = 17, a(2) = 197, a(3) = 1957 For n ≥
 *            4: a(n) = 17*a(n-1) - 92*a(n-2) + 172*a(n-3) - 96*a(n-4)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)17;
    if (n == 2) return (seq_t)197;
    if (n == 3) return (seq_t)1957;

    /* a0 = a(n-4), a1 = a(n-3), a2 = a(n-2), a3 = a(n-1) */
    seq_t a0 = (seq_t)1;      /* a(0)   */
    seq_t a1 = (seq_t)17;     /* a(1)   */
    seq_t a2 = (seq_t)197;    /* a(2)   */
    seq_t a3 = (seq_t)1957;   /* a(3)   */

    for (idx_t i = 4; i <= n; ++i)
    {
        seq_t next = 17 * a3 - 92 * a2 + 172 * a1 - 96 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }

    return a3;
}
