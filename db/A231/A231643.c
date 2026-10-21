/*
 * A231643  a(n) = 5*2^n + 5.
 * https://oeis.org/A231643
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*2^n + 5 Equivalent recurrence: a(0)=10, a(1)=15,
 *            a(n)=3*a(n-1)-2*a(n-2) for n>1
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)10;
    if (n == 1)
        return (seq_t)15;

    seq_t a0 = (seq_t)10;   /* a(0) */
    seq_t a1 = (seq_t)15;   /* a(1) */
    seq_t an;

    for (idx_t i = 2; i <= n; ++i) {
        an = 3 * a1 - 2 * a0;
        a0 = a1;
        a1 = an;
    }
    return a1;  /* a(n) */
}
