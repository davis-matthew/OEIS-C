/*
 * A137742  a(n) = (n-1)*(n+4)*(n+6)/6 for n > 1, a(1)=1.
 * https://oeis.org/A137742
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1)*(n+4)*(n+6)/6, with a(1)=1
 * Range:     u8 n <= 8, u16 n <= 70, u32 n <= 2087, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;          /* undefined for n<1, return 0 as placeholder */

    if (n == 1)
        return (seq_t)1;

    seq_t nn = (seq_t)n;
    seq_t term = (nn - 1) * (nn + 4) * (nn + 6) / (seq_t)6;
    return term;
}
