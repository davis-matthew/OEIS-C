/*
 * A034788  a(n) is the n-th sextic factorial number divided by 6.
 * https://oeis.org/A034788
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 6^(n-1) * n! (n ≥ 1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1; a(n)=6*n*a(n-1) */

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;

    seq_t res = (seq_t)1;
    for (idx_t i = (idx_t)2; i <= n; ++i)
        res *= (seq_t)6 * (seq_t)i;

    return res;
}
