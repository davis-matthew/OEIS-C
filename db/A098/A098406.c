/*
 * A098406  a(n) = (10^n + 17)/9.
 * https://oeis.org/A098406
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10^n + 17)/9 Recurrence: a(0)=2, a(n)=10*a(n-1)-17 (for
 *            n≥1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)2;

    seq_t res = (seq_t)2;
    for (idx_t i = (idx_t)1; i <= n; ++i)
        res = res * (seq_t)10 - (seq_t)17;
    return res;
}
