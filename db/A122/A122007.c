/*
 * A122007  Expansion of 2*x^2*(1-2*x) / ((3*x-1)*(3*x^2-1)).
 * https://oeis.org/A122007
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 3*a(n-2) - 9*a(n-3); Initial values: a(1)=0,
 *            a(2)=2, a(3)=2
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 22, u64 n <= 42, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)0;
    if (n == 2 || n == 3)
        return (seq_t)2;

    /* a1 = a(n-3), a2 = a(n-2), a3 = a(n-1) */
    seq_t a1 = (seq_t)0;          /* a(1) */
    seq_t a2 = (seq_t)2;          /* a(2) */
    seq_t a3 = (seq_t)2;          /* a(3) */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t an = 3 * a3 + 3 * a2 - 9 * a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return a3;
}
